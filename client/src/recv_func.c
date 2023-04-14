#include "../inc/client.h"
extern int in_chat;
void print_message(char *login, char *message);

void *recv_func()
{
    char command[2048];

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while (1)
    {
        mx_printstr("recv\n");
        pthread_mutex_lock(&mutex1);

        while (!in_chat)
        {
            pthread_cond_wait(&new_data_cond, &mutex1);
        }
        int len = SSL_read(current_client.ssl, command, sizeof(command) - 1);

        pthread_mutex_unlock(&mutex1);
        if (len == -1)
        {
            printf("Error receiving message\n");
            break;
        }
        cJSON *json_obj = cJSON_Parse(command);
        memset(command, 0, sizeof(command));
        if (!json_obj)
        {
            printf("Error: Invalid JSON data received from server\n");
            break;
        }
        // Извлечение данных из JSON-объекта
        // char *login = cJSON_GetObjectItemCaseSensitive(json_obj, "name")->valuestring;
        // char *message = cJSON_GetObjectItemCaseSensitive(json_obj, "message")->valuestring;
        // print_message(login, message);
        cJSON_Delete(json_obj);
        // data_ready = 0;
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
