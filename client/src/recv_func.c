#include "../inc/client.h"
void print_message(char *login, char *message);

void *recv_func()
{
    char command[2048];
    while (!main_client.connected)
    {
        int i = 0;
        i++;
    }
    while (main_client.connected)
    {
        // pthread_mutex_lock(&mutex1);
        int len = SSL_read(current_client.ssl, command, sizeof(command) - 1);
        // pthread_mutex_unlock(&mutex1);
        if (len == -1)
        {
            printf("Error receiving message\n");
            break;
        }
        if (mx_strcmp(command, "<user_list>") == 0)
        {

            t_list *ccc = receive_list(current_client.ssl);

            show_create_new_chat_with_someone(ccc);

            // while (ccc != NULL)
            // {
            //     t_list *tmp = ccc;
            //     ccc = ccc->next;
            //     free(tmp->data);
            //     free(tmp);
            // }
        }
        else if (mx_strcmp(command, "<friend_list>") == 0)
        {
            t_list *friend_list = receive_list(current_client.ssl);

            t_list *current = friend_list;

            while (current)
            {
                show_chats_with_added_friends(((t_user *)current->data)->username);
                mx_printstr(((t_user *)current->data)->username);
                mx_printstr("\n");
                current = current->next;
            }

            while (friend_list != NULL)
            {
                t_list *tmp = friend_list;
                friend_list = friend_list->next;
                free(tmp->data);
                free(tmp);
            }
        }
        else if (mx_strcmp(command, "<add_friend>") == 0)
        {
            char friendname[32];
            int rec = SSL_read(current_client.ssl, friendname, sizeof(command) - 1);
            if (rec == -1)
            {
                printf("Error receiving message\n");
            }

            if (mx_strcmp(friendname, "User not found, write friend's login correct") == 0)
            {
                mx_printstr("User not found, write friend's login correct");
                break;
            }
        }
        // else if (mx_strcmp(command, "<send_message_to>") == 0)
        // {
        //     t_list *friend_list = receive_list(current_client.ssl);

        //     t_list *current = friend_list;
        // }
        else if (mx_strcmp(command, "<recv_message_from>") == 0)
        {
            cJSON *json_obj = cJSON_Parse(command);
            memset(command, 0, sizeof(command));
            if (!json_obj)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            // Извлечение данных из JSON-объекта
            char *login = cJSON_GetObjectItemCaseSensitive(json_obj, "name")->valuestring;
            char *message = cJSON_GetObjectItemCaseSensitive(json_obj, "message")->valuestring;
            print_message(login, message);
            cJSON_Delete(json_obj);
        }
        else if (mx_strcmp(command, "<show_history>") == 0)
        {
            const int temp_size = 4096;
            char temp[temp_size];

            int bytes_received = SSL_read(current_client.ssl, temp, temp_size - 1);
            if (bytes_received <= 0)
            {
                return NULL;
            }

            temp[bytes_received] = '\0';

            if (mx_strcmp(temp, "<chat empty>") == 0) {
                continue;
            }
            
            t_list *chat_history = deserialize_chathistory_list(temp);

            t_list *current = chat_history;

            while (current)
            {
                // show_chats_with_friends(((t_user *)current->data)->username);
                mx_printstr(((t_chat *)current->data)->sender);
                mx_printstr(" -> ");
                mx_printstr(((t_chat *)current->data)->message);
                mx_printstr(" | ");
                mx_printstr(((t_chat *)current->data)->timestamp);
                mx_printchar('\n');
                current = current->next;
            }

            while (chat_history != NULL)
            {
                t_list *tmp = chat_history;
                chat_history = chat_history->next;
                free(tmp->data);
                free(tmp);
            }
        }
        // else
        // {
        //     // Преобразование строки в JSON-объект
        //     cJSON *json_obj = cJSON_Parse(command);
        //     memset(command, 0, sizeof(command));
        //     if (!json_obj)
        //     {
        //         printf("Error: Invalid JSON data received from server\n");
        //         break;
        //     }
        //     // Извлечение данных из JSON-объекта
        //     char *login = cJSON_GetObjectItemCaseSensitive(json_obj, "name")->valuestring;
        //     char *message = cJSON_GetObjectItemCaseSensitive(json_obj, "message")->valuestring;
        //     print_message(login, message);
        //     cJSON_Delete(json_obj);
        // }
    }
    pthread_mutex_unlock(&mutex2);
    pthread_detach(pthread_self());
    return NULL;
}

void print_message(char *login, char *message)
{
    mx_printstr(login);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}
