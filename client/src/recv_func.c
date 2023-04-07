#include "../inc/client.h"
void print_message(char *login, char *message);

void *recv_func()
{
    char temp[2048];
    // while (main_client.connected == false)
    // {
    //     int len = SSL_read(current_client.ssl, temp, sizeof(temp) - 1);
    //     if (len == -1)
    //     {
    //         printf("Error receiving message\n");
    //     }
    //     if(mx_strcmp(temp, "success\n") == 0) {
    //         main_client.connected = true;
    //         break;
    //     }
    // }
    // mx_printint(main_client.connected);
    while (!main_client.connected) {
        int i = 0;
        i++;
        // mx_printint(main_client.connected);
        // sleep(60);
    }
    while (main_client.connected)
    {
        pthread_mutex_lock(&mutex1);
        int len = SSL_read(current_client.ssl, temp, sizeof(temp) - 1);
        pthread_mutex_unlock(&mutex1);
        if (len == -1)
        {
            printf("Error receiving message\n");
        }
        if (mx_strcmp(temp, "<user_list>") == 0)
        {
            t_list *user_list = receive_list(current_client.ssl);

            t_list *ccc = user_list;

            show_create_new_chat_with_someone(user_list);

            while (user_list != NULL)
            {
                t_list *tmp = user_list;
                user_list = user_list->next;
                free(tmp->data);
                free(tmp);
            }
        }
        else if (mx_strcmp(temp, "<friend_list>") == 0)
        {
            t_list *friend_list = receive_list(current_client.ssl);

            t_list *current = friend_list;
    
            while (current) {
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
        else if (mx_strcmp(temp, "<add_friend>") == 0)
        {
            mx_printstr("Enter friend's login: ");

            int rec = SSL_read(current_client.ssl, temp, sizeof(temp) - 1);
            if (rec == -1)
            {
                printf("Error receiving message\n");
            }

            if (mx_strcmp(temp, "User not found, write friend's login correct") == 0)
            {
                mx_printstr("User not found, write friend's login correct");
                break;
            }
            else
            {
                mx_printstr(temp);
                mx_printstr("\n\n");
            }
        }
        else
        {
            // Преобразование строки в JSON-объект
            cJSON *json_obj = cJSON_Parse(temp);
            memset(temp, 0, sizeof(temp));
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
