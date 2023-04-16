#include "../inc/client.h"
extern int in_chat;
t_list *friend_list;
t_list *user_list;
t_list *chat_history;

void print_message(char *login, char *message);

// void *recv_func()
// {
//     char command[2048];
//     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//     while (1)
//     {
//         mx_printstr("recv\n");
//         pthread_mutex_lock(&recv);
//         while (!in_chat)
//         {
//             pthread_cond_wait(&new_data_cond, &recv);
//         }
//         in_chat = 0;
//         pthread_mutex_unlock(&recv);
//         int len = SSL_read(current_client.ssl, command, sizeof(command) - 1);
//         if (len == -1)
//         {
//             printf("Error receiving message\n");
//             break;
//         }
//         cJSON *json_obj = cJSON_Parse(command);
//         memset(command, 0, sizeof(command));
//         if (!json_obj)
//         {
//             printf("Error: Invalid JSON data received from server\n");
//             break;
//         }
//         //Извлечение данных из JSON-объекта
//         char *login = cJSON_GetObjectItemCaseSensitive(json_obj, "name")->valuestring;
//         char *message = cJSON_GetObjectItemCaseSensitive(json_obj, "message")->valuestring;
//         receive_msg(message);
//         //print_message(login, message);
//         cJSON_Delete(json_obj);
//         // data_ready = 0;
//     }
//     return NULL;
// }

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
        printf("Thread recv_func trying to lock mutex\n");
        pthread_mutex_lock(&mutex_recv);
        int len = SSL_read(current_client.ssl, command, sizeof(command));
        pthread_mutex_unlock(&mutex_recv);
        printf("Thread recv_func trying to unlock mutex\n");
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (mx_strcmp(command, "<user_list>") == 0)
        {
            user_list = receive_list(current_client.ssl);
            // mx_printstr(((t_user*)user_list->data)->username);
            // if (user_list == NULL)
            // {
            //     printf("null\n");
            //     break;
            // }
        }
        else if (mx_strcmp(command, "<friend_list>") == 0)
        {
            friend_list = receive_list(current_client.ssl);
        }
        else if (mx_strcmp(command, "<add_friend>") == 0)
        {
            char temp[128];
            pthread_mutex_lock(&mutex_recv);
            int len = SSL_read(current_client.ssl, temp, sizeof(temp));
            pthread_mutex_unlock(&mutex_recv);
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
            char temp[128];
            pthread_mutex_lock(&mutex_recv);
            int len = SSL_read(current_client.ssl, temp, sizeof(temp));
            pthread_mutex_unlock(&mutex_recv);
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
            char *sender = cJSON_GetObjectItemCaseSensitive(json, "sender")->valuestring;
            mx_printstr(sender);
        }
        else if (mx_strcmp(command, "<show_history>") == 0)
        {
            char temp[128];
            pthread_mutex_lock(&mutex_recv);
            int len = SSL_read(current_client.ssl, temp, sizeof(temp));
            pthread_mutex_unlock(&mutex_recv);
            if (len < 0)
            {
                printf("Error: Unable to receive data from server\n");
                break;
            }
            if (mx_strcmp(temp, "chat empty") == 0)
            {
                mx_printstr("chat empty");
                continue;
            }
            chat_history = deserialize_chathistory_list(temp);
        }
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
