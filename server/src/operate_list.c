#include "../inc/server.h"

int send_list(SSL *ssl, t_list *head) {
    char *serialized_list = serialize_list(head);
    int result = SSL_write(ssl, serialized_list, strlen(serialized_list));
    free(serialized_list);
    return result;
}

char* serialize_list(t_list* head) {
    cJSON *json_list = cJSON_CreateArray();
    
    while (head != NULL) {
        cJSON *json_node = cJSON_CreateObject();
        cJSON_AddStringToObject(json_node, "name", (char *)head->data);
        cJSON_AddItemToArray(json_list, json_node);
        head = head->next;
    }
    
    char *serialized_list = cJSON_Print(json_list);
    cJSON_Delete(json_list);
    return serialized_list;
}

void send_message_to_all_clients(char *message, int current_socket, char *login)
{
    /* Блокируем доступ к списку клиентов, пока идет отправка сообщений */
    char *json_str = convert_to_json(message, login);
    pthread_mutex_lock(&clients_mutex);
    t_list *current = users_list;
    while (current != NULL)
    {
        // if (((t_client *)current->data)->cl_socket != current_socket && ((t_client *)current->data)->connected == true)
        if (((t_client *)current->data)->cl_socket != current_socket && ((t_client *)current->data)->connected == true)
        {
            SSL *ssl = ((t_client *)current->data)->ssl;
            SSL_write(ssl, json_str, mx_strlen(json_str));
        }
        current = current->next;
    }
    pthread_mutex_unlock(&clients_mutex);
    memset(json_str, 0, mx_strlen(json_str));
}

char *convert_to_json(char *buffer, char *login)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", login);
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

