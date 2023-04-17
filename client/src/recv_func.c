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
        // printf("\nThread recv_func trying to lock mutex\n");
        // pthread_mutex_lock(&mutex_recv);
        int len = SSL_read(current_client.ssl, command, sizeof(command));
        // pthread_mutex_unlock(&mutex_recv);
        printf("\n%d ->", len);
        printf(" %s\n", command);
        // printf("Thread recv_func trying to unlock mutex\n");
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (mx_strcmp(command, "<user_list>") == 0)
        {
            user_list = receive_list(current_client.ssl);
            // printf("%s\n", ((t_user *)user_list->data)->username);
            show_user_list_scrolled(user_list);
        }
        else if (mx_strcmp(command, "<friend_list>") == 0)
        {
            char temp[4196];
            int cmd = SSL_read(current_client.ssl, temp, sizeof(temp));
            if (cmd <= 0)
            {
                int error_code = SSL_get_error(current_client.ssl, cmd);
                fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                break;
            }
            if (mx_strcmp("User has no friends", temp) == 0)
            {
                break;
            }
            cJSON *received_json = cJSON_Parse(temp);
            if (received_json == NULL)
            {
                fprintf(stderr, "Error parsing JSON string: %s\n", cJSON_GetErrorPtr());
                break;
            }

            friend_list = process_json_object(received_json);

            t_list *current = friend_list;
        
            t_Friend *friend_data = (t_Friend *)current->data;

            while (current != NULL)
            {
                t_Friend *friend_data = (t_Friend *)current->data;
                show_chats_with_added_friends(friend_data->username);
                current = current->next;
            }
            cJSON_Delete(received_json);
        }
        else if (mx_strcmp(command, "<add_friend>") == 0)
        {
            char temp[128];
            // pthread_mutex_lock(&mutex_recv);
            int len = SSL_read(current_client.ssl, temp, sizeof(temp));
            // pthread_mutex_unlock(&mutex_recv);
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
            mx_printstr(friendname);
        }
        else if (mx_strcmp(command, "<send_message>") == 0)
        {
            char temp[256];
            // pthread_mutex_lock(&mutex_recv);
            int len = SSL_read(current_client.ssl, temp, sizeof(temp));
            // pthread_mutex_unlock(&mutex_recv);
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                break;
            }
            cJSON *json = cJSON_Parse(temp);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            t_chat *message_data = NULL;
            message_data->sender = cJSON_GetObjectItemCaseSensitive(json, "sender")->valuestring;
            message_data->message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
            message_data->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
            message_data->timestamp = cJSON_GetObjectItemCaseSensitive(json, "timestamp")->valuestring;
        }
        // else if (mx_strcmp(command, "<show_history>") == 0)
        // {
        //     char temp[128];
        //     // pthread_mutex_lock(&mutex_recv);
        //     int len = SSL_read(current_client.ssl, temp, sizeof(temp));
        //     // pthread_mutex_unlock(&mutex_recv);
        //     if (len < 0)
        //     {
        //         printf("Error: Unable to receive data from server\n");
        //         break;
        //     }
        //     if (mx_strcmp(temp, "chat empty") == 0)
        //     {
        //         mx_printstr("chat empty");
        //         continue;
        //     }
        //     chat_history = deserialize_chathistory_list(temp);
        // }
        // else if (mx_strcmp(command, "<delete_message>") == 0)
        // {
        //     char temp[128];
        //     // pthread_mutex_lock(&mutex_recv);
        //     int len = SSL_read(current_client.ssl, temp, sizeof(temp));
        // }
    }
    // g_free(command);
    return NULL;
}

void print_message(char *login, char *message)
{
    mx_printstr(login);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}
