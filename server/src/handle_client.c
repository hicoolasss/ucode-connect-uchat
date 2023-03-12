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
        memset(buf, 0, sizeof(buf));
        int len = SSL_read(current_client->ssl, buf, sizeof(buf));
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
            char *status = cJSON_GetObjectItemCaseSensitive(json, "status")->valuestring;
            // current_client->login = mx_strdup(login);
            // current_client->passwd = mx_strdup(passwd);

            // other func db_regestr_to_serv();
            if (mx_atoi(status) == 0)
            {
                int db_log = db_log_to_serv(login, passwd, current_client->ssl);
                if (db_log == 1)
                {
                    SSL_write(current_client->ssl, "user not found\n", 20);
                }
                else if (db_log == 2)
                {
                    SSL_write(current_client->ssl, "incorrect password\n", 20);
                }
                else if (db_log == 0)
                {
                    SSL_write(current_client->ssl, "success\n", 9);
                    mx_printstr(login);
                    mx_printstr(" success\n");
                    memset(login, 0, mx_strlen(login));
                    memset(passwd, 0, mx_strlen(passwd));
                    main_client.registered = true;
                }
            }
            else if (mx_atoi(status) == 1)
            {
                int db_reg = db_regestr_to_serv(login, passwd, current_client->ssl);
                if (db_reg == 1)
                {
                    SSL_write(current_client->ssl, "this user already exist\n", 25);
                }
                else if (db_reg == 0)
                {
                    SSL_write(current_client->ssl, "registered\n", 12);
                    mx_printstr(login);
                    mx_printstr(" registered\n");
                    memset(login, 0, mx_strlen(login));
                    memset(passwd, 0, mx_strlen(passwd));
                }
            }
        }
    }
    while (main_client.registered == true)
    {

        memset(buf, 0, sizeof(buf));
        int len = SSL_read(current_client->ssl, buf, sizeof(buf));
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

            char *login = cJSON_GetObjectItemCaseSensitive(json, "login")->valuestring;
            char *command = cJSON_GetObjectItemCaseSensitive(json, "command")->valuestring;

            if (mx_strcmp(command, "<logout>") == 0)
            {
                print_message(login, "logout\n");
                remove_client(current_client->cl_socket);
                cli_count--;
                SSL_write(current_client->ssl, "success\n", 9);
                main_client.registered = false;
            }
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
