#include "../inc/server.h"

int recv_all(SSL *sockfd, char *buf, int len);
int send_all(SSL *sockfd, char *buf, int len);
void remove_client(int socket_fd);
void send_message_to_all_clients(char *message, int current_socket);
void print_message(t_client *client, char *message);

void *handle_client(void *args)
{

    t_client *current_client = (t_client *)args;
    // char message[2048];
    char buf[2048];
    while (1)
    {
        int len = SSL_read(current_client->ssl, buf, sizeof(buf) - 1);
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (len == 0)
        {
            printf("Connection closed by server\n");
            break;
        }
        else
        {
            // buf[len] = '\0';
            // printf("Data received from server: %s\n", buf);

            // Преобразование строки в JSON-объект
            cJSON *json = cJSON_Parse(buf);
            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }
            // Извлечение данных из JSON-объекта
            char *name = cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring;
            current_client->login = mx_strdup(name);
            char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
            print_message(current_client, message);
            send_message_to_all_clients(message, current_client->cl_socket);
            // mx_printstr(message);
        }
    }
    return NULL;
}

void print_message(t_client *client, char *message)
{
    mx_printstr(client->login);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}

void send_message_to_all_clients(char *message, int current_socket)
{
    /* Блокируем доступ к списку клиентов, пока идет отправка сообщений */
    pthread_mutex_lock(&clients_mutex);
    t_list *current = users_list;
    while (current != NULL)
    {
        SSL *ssl = ((t_client *)current->data)->ssl;
        SSL_write(ssl, message, strlen(message));
        current = current->next;
    }
    pthread_mutex_unlock(&clients_mutex);
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
