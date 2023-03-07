#include "../inc/server.h"

int recv_all(SSL *sockfd, char *buf, int len);
int send_all(SSL *sockfd, char *buf, int len);
void remove_client(int socket_fd);
void send_message_to_all_clients(char *message, int current_socket);
void print_message(char *login, char *message);
char *convert_to_json(char *buffer);

t_main main_client;
t_client *current_client;

void *handle_client(void *args)
{

    current_client = (t_client *)args;
    // char message[2048];
    char buf[2048];
    main_client.registered = false;

    while (main_client.registered == false)
    {
        int len = SSL_read(current_client->ssl, buf, sizeof(buf) - 1);
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else
        {
            // Преобразование строки в JSON-объект
            cJSON *json = cJSON_Parse(buf);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            // Извлечение данных из JSON-объекта
            
            char *login = cJSON_GetObjectItemCaseSensitive(json, "login")->valuestring;
            char *passwd = cJSON_GetObjectItemCaseSensitive(json, "password")->valuestring;
            current_client->login = mx_strdup(login);
            current_client->passwd = mx_strdup(passwd);

           

            //other func db_regestr_to_serv();
            if (db_log_to_serv(current_client->login, current_client->passwd, current_client->ssl) == 1) {
               SSL_write(current_client->ssl, "incorrect password\n", 20);
            } else {
                SSL_write(current_client->ssl, "success\n", 9);
                mx_printstr(current_client->login);
                mx_printstr(" success\n");
                memset(login, 0, mx_strlen(login));
                memset(passwd, 0, mx_strlen(passwd));
                main_client.registered = true;
            }
        }
    }
    while (main_client.registered == true)
    {
        int len = SSL_read(current_client->ssl, buf, sizeof(buf) - 1);
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (len == 0)
        {
            mx_printstr(current_client->login);
            cli_count--;
            printf(" disconnected\n");
            break;
        }
        else
        {
            // buf[len] = '\0';
            printf("Data received from server: %s\n", buf);

            // Преобразование строки в JSON-объект
            cJSON *json_obj = cJSON_Parse(buf);
            if (!json_obj)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            // Извлечение данных из JSON-объекта
            char *message = cJSON_GetObjectItemCaseSensitive(json_obj, "message")->valuestring;
            char *login = cJSON_GetObjectItemCaseSensitive(json_obj, "name")->valuestring;
            print_message(login, message);
            send_message_to_all_clients(message, current_client->cl_socket);
            // mx_printstr(message);
            cJSON_Delete(json_obj);
        }
    }
    return NULL;
}

void print_message(char *login, char *message)
{
    mx_printstr(login);
    mx_printstr(" -> ");
    mx_printstr(message);current_client->login = mx_strdup(login);
    mx_printchar('\n');
}

void send_message_to_all_clients(char *message, int current_socket)
{
    /* Блокируем доступ к списку клиентов, пока идет отправка сообщений */
    char *json_str = convert_to_json(message);
    pthread_mutex_lock(&clients_mutex);
    t_list *current = users_list;
    while (current != NULL)
    {
        if (((t_client *)current->data)->cl_socket != current_socket)
        {
            SSL *ssl = ((t_client *)current->data)->ssl;
            SSL_write(ssl, json_str, mx_strlen(json_str));
        }
        current = current->next;
    }
    pthread_mutex_unlock(&clients_mutex);
    memset(json_str, 0, mx_strlen(json_str));
}

char *convert_to_json(char *buffer)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", current_client->login);
    // cJSON_AddStringToObject(json, "password", cur_client.passwd);
    cJSON_AddStringToObject(json, "message", buffer);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
}

void remove_client(int socket_fd)
{
    /* Ищем элемент списка с заданным файловым дескриптором */
    t_list *cur = users_list;
    t_list *prev_client = NULL;
    t_client *cl = ((t_client *)(cur->data));
    while (cur != NULL && cl->cl_socket != socket_fd)
    {
        prev_client = cur;
        cur = cur->next;
    }

    /* Если элемент найден, удаляем его из списка */
    if (cur != NULL)
    {
        if (prev_client == NULL)
        {
            /* Удаляем первый элемент списка */
            cur = cur->next;
        }
        else
        {
            /* Удаляем не первый элемент списка */
            prev_client->next = cur->next;
        }
        /* Освобождаем память, занятую удаляемым элементом */
        free(cur);
    }
}

int send_all(SSL *sockfd, char *buf, int len)
{
    ssize_t n;

    while (len > 0)
    {
        n = SSL_write(sockfd, buf, len);
        if (n <= 0)
            return -1;
        buf += n;
        len -= n;
    }

    return 1;
}

int recv_all(SSL *sockfd, char *buf, int len)
{

    ssize_t n;

    while (len > 0)
    {
        n = SSL_read(sockfd, buf, len);
        if (n <= 0)
            return -1;
        buf += n;
        len -= n;
    }

    return 1;
}
