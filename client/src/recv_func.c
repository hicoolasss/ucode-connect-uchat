#include "../inc/client.h"
extern int in_chat;
t_list *friend_list;
t_list *user_list;
t_list *chat_history;

void print_message(char *login, char *message);

gpointer recv_func(gpointer data)
{
    char command[2048];

    pthread_mutex_lock(&mutex_recv);
    while (!main_client.connected)
    {
        pthread_cond_wait(&auth_cond, &mutex_recv);
    }
    // Здесь пользователь авторизован, и поток может продолжить работу с мьютексом
    pthread_mutex_unlock(&mutex_recv);

    while (running)
    {
        memset(command, 0, sizeof(command));
        int len = stable_recv(current_client.ssl, command, sizeof(command));
        printf("\n%d ->", len);
        printf(" %s\n", command);
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (mx_strcmp(command, "<user_list>") == 0)
        {
            user_list = receive_list(current_client.ssl);
            show_user_list_scrolled(user_list);
        }
        else if (mx_strcmp(command, "<friend_list>") == 0)
        {
            char temp[21474];
            int cmd = stable_recv(current_client.ssl, temp, sizeof(temp));
            mx_printstr(temp);
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

            add_new_friend(&friend_list, friendname);
            // mx_printstr(friendname);
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
            message_data->timestamp = mx_strdup(json_message_timestamp->valuestring);
            
            add_message_to_chat_history(&friend_list, friendname, message_data);

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
            running = false;
            // break;
        }
        else if (mx_strcmp(command, "<create_group>") == 0)
        {
            char temp[16784];
            char *group_name;
            int len = stable_recv(current_client.ssl, temp, sizeof(temp));
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                break;
            }
            printf("\n%s\n", temp);
            cJSON *json_group = cJSON_Parse(temp);
            if (!json_group)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            
            t_list *group_list = extract_group_and_friends_from_json(json_group, &group_name);
        }
    }
    return NULL;
}

void print_message(char *login, char *message)
{
    mx_printstr(login);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}
