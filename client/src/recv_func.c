#include "../inc/client.h"
void print_message(char *login, char *message);

void *recv_func()
{
    char temp[2048];
    // while (main_client.connected == false)
    // {
    //     int len = SSL_read(cur_client.ssl, temp, sizeof(temp) - 1);
    //     if (len == -1)
    //     {
    //         printf("Error receiving message\n");
    //     }
    //     if(mx_strcmp(temp, "success\n") == 0) {
    //         pthread_mutex_lock(&cl_mutex);
    //         main_client.connected = true;
    //         registered = true;
    //         pthread_mutex_unlock(&cl_mutex);
    //         break;
    //     }
    // }
    while (1)
    {
        int len = SSL_read(cur_client.ssl, temp, sizeof(temp) - 1);
        if (len == -1)
        {
            printf("Error receiving message\n");
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
