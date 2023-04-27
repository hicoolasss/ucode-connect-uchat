#include "../inc/client.h"
extern int in_chat;
t_list *friend_list;
t_list *user_list;
t_list *chat_history;
extern t_achievements current_achievements;

volatile gboolean running = TRUE;

void print_message(char *login, char *message);

gpointer recv_func()
{
    char command[2048];

    while (running)
    {
        printf("here\n");
        memset(command, 0, sizeof(command));
        int len = SSL_read(current_client.ssl, command, sizeof(command));
        printf("\n%d ->", len);
        printf(" %s\n", command);
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (mx_strcmp(command, "<Offline>") == 0)
        {
            char *temp[32];
            int cmd = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (cmd <= 0)
            {
                int error_code = SSL_get_error(current_client.ssl, cmd);
                fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                break;
            }
            t_list *current = user_list;
            while (current)
            {
                if(mx_strcmp(((t_user*)current->data)->username, (const char*)temp) == 0)
                {
                    ((t_user*)current->data)->connected = false;
                }
                current = current->next;
            }
        }
        else if (mx_strcmp(command, "<user_list>") == 0)
        {
            user_list = receive_list(current_client.ssl);
            t_list *current = user_list;
            while (current)
            {
                printf("%d", ((t_user *)current->data)->connected);
                current = current->next;
            }
            show_user_list_scrolled(user_list);
        }
        else if (mx_strcmp(command, "<friend_list>") == 0)
        {
            char temp[21474];
            int cmd = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (cmd <= 0)
            {
                int error_code = SSL_get_error(current_client.ssl, cmd);
                fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                break;
            }
            if (mx_strcmp("User has no friends", temp) == 0)
            {
                continue;
            }
            cJSON *received_json = cJSON_Parse(temp);
            if (received_json == NULL)
            {
                fprintf(stderr, "Error parsing JSON string: %s\n", cJSON_GetErrorPtr());
                break;
            }
            friend_list = process_json_object(received_json);

            show_chats_with_added_friends(friend_list);

            cJSON_Delete(received_json);
        }
        else if (mx_strcmp(command, "<add_friend>") == 0)
        {
            char temp[128];
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                break;
            }
            if (mx_strcmp(temp, "already your friend") == 0)
            {
                mx_printstr("already your friend\n");
                continue;
            }
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;
            char *avatarname = cJSON_GetObjectItemCaseSensitive(json, "avatarname")->valuestring;
            add_new_friend(&friend_list, friendname, avatarname);
        }
        else if (mx_strcmp(command, "<send_message_in_chat>") == 0)
        {
            char temp[16784];
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                break;
            }
            printf("\n%s\n", temp);
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }

            cJSON *json_message_id = cJSON_GetObjectItem(json, "id");
            cJSON *json_message_text = cJSON_GetObjectItem(json, "message");
            cJSON *json_message_timestamp = cJSON_GetObjectItem(json, "timestamp");
            cJSON *json_sender = cJSON_GetObjectItem(json, "sender");
            char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;

            t_chat *message_data = (t_chat *)malloc(sizeof(t_chat));
            message_data->sender = mx_strdup(json_sender->valuestring);
            message_data->message = mx_strdup(json_message_text->valuestring);
            message_data->id = json_message_id->valueint;
            

            if (message_data->id > 5000) {
                current_achievements.milka = true;
                update_show_achievements();
            }


            message_data->timestamp = mx_strdup(json_message_timestamp->valuestring);

            t_list *new_node = add_message_to_chat_history(&friend_list, friendname, message_data);

            if (new_node && new_node->data)
            {
                t_chat *chat = (t_chat *)new_node->data;
                update_current_chat(chat, friendname);
                // printf("%s\n", chat->message);
            }
            t_chat *data = message_data;
            if (data)
            {
                if (data->message)
                {
                    free(data->message);
                }
                if (data->timestamp)
                {
                    free(data->timestamp);
                }
                if (data->sender)
                {
                    free(data->sender);
                }
                free(data);
            }
            memset(temp, 0, sizeof(temp));
        }
        else if (mx_strcmp(command, "<logout>") == 0)
        {
            pthread_mutex_lock(&mutex_recv);
            clear_friend_list(friend_list);
            clear_user_list(user_list);
            printf("Exit\n");
            main_client.connected = false;
            running = false;
            pthread_mutex_unlock(&mutex_recv);
            g_thread_exit(NULL);
        }
        else if (mx_strcmp(command, "<update_image>") == 0)
        {
            char temp[16784];
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                continue;
            }
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                continue;
            }
            char *username = cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring;
            char *avatarname = cJSON_GetObjectItemCaseSensitive(json, "avatarname")->valuestring;

            int result = update_user_avatar(user_list, username, avatarname);
            if (result != 0)
            {
                printf("User not found\n");
            }
            result = update_user_avatar(friend_list, username, avatarname);
            if (result != 0)
            {
                printf("User not found\n");
            }
        }
        else if (mx_strcmp(command, "<delete_message_in_chat>") == 0)
        {
            char temp[8392];
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                continue;
            }
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                continue;
            }
            char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
            char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;
            int message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id")->valueint;

            delete_message(friend_list, friendname, message_id, message);
        }
        else if (mx_strcmp(command, "<update_message_in_chat>") == 0)
        {
            char temp[8392];
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                continue;
            }
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                continue;
            }
            char *message = cJSON_GetObjectItemCaseSensitive(json, "new_message")->valuestring;
            char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;
            int message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id")->valueint;

            update_message(friend_list, friendname, message_id, message);
        }
    }
    g_print("Поток завершен.\n");
    return NULL;
}

void print_message(char *login, char *message)
{
    mx_printstr(login);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}
