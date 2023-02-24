#include "../inc/server.h"

void send_message_to_all_clients(char *message, int socket);
void remove_client(int socket_fd);
void print_message(t_client *client, char *message);

void *handle_client(void *args)
{
    t_chat current_chat;
    t_client *current_client = (t_client *)args;
    while (1)
    {
        //SSL_write(current_client->ssl, message, sizeof(message) - 1);
        memset(current_chat.message, 0, sizeof(*current_chat.message));
        int len = SSL_read(current_client->ssl, current_chat.message, sizeof(*current_chat.message) - 1);
        
        if (len > 0)
        {   
            print_message(current_client, current_chat.message);
            // send_message_to_all_clients(message, current_client->cl_socket);
            memset(current_chat.message, 0, sizeof(*current_chat.message));
        }
        else
        {
            printf("Client disconnected.\n");
            memset(current_chat.message, 0, sizeof(*current_chat.message));
            break;
        }
    }
    remove_client(current_client->cl_socket);
    cli_count--;
    // free_client(&current_client, &users_list);
    SSL_free(current_client->ssl);
    pthread_exit(NULL);
}

void print_message(t_client *client, char *message) {
    mx_printint(client->id);
    mx_printstr(" -> ");
    mx_printstr(message);
    mx_printchar('\n');
}
void send_message_to_all_clients(char *message, int socket)
{
    /* Блокируем доступ к списку клиентов, пока идет отправка сообщений */
    pthread_mutex_lock(&clients_mutex);

    /* Итерируемся по всем клиентам и отправляем им сообщение */
    t_list *cur = users_list;
    while (cur)
    {
        if (((t_client *)cur->data)->cl_socket > 0 && ((t_client *)cur->data)->cl_socket != socket) {
            SSL_write(((t_client *)cur->data)->ssl, message, mx_strlen(message));
            cur = cur->next;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int socket_fd) {
    /* Ищем элемент списка с заданным файловым дескриптором */
    t_list *cur = users_list;
    t_list *prev_client = NULL;
	t_client *cl = ((t_client *)(cur->data));
    while ( cur!= NULL && cl->cl_socket != socket_fd) {
        prev_client = cur;
        cur = cur->next;
    }

    /* Если элемент найден, удаляем его из списка */
    if (cur != NULL) {
        if (prev_client == NULL) {
            /* Удаляем первый элемент списка */
            cur = cur->next;
        } else {
            /* Удаляем не первый элемент списка */
            prev_client->next = cur->next;
        }
        /* Освобождаем память, занятую удаляемым элементом */
        free(cur);
    }
}
