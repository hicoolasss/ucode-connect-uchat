#include "../inc/server.h"

int send_namelist(SSL *ssl, t_list *head)
{
    char *serialized_list = serialize_namelist(head);
    int result = SSL_write(ssl, serialized_list, strlen(serialized_list));
    free(serialized_list);
    return result;
}

char *serialize_namelist(t_list *head)
{
    cJSON *json_list = cJSON_CreateArray();

    while (head != NULL)
    {
        cJSON *json_node = cJSON_CreateObject();
        cJSON_AddStringToObject(json_node, "name", ((t_user *)head->data)->username);
        cJSON_AddItemToArray(json_list, json_node);
        head = head->next;
    }

    char *serialized_list = cJSON_Print(json_list);
    cJSON_Delete(json_list);
    return serialized_list;
}

char *serialize_historylist(t_list *head)
{
    cJSON *json_list = cJSON_CreateArray();

    while (head != NULL)
    {
        cJSON *json_node = cJSON_CreateObject();
        cJSON_AddNumberToObject(json_node, "message_id", ((t_chat *)head->data)->id);
        cJSON_AddStringToObject(json_node, "sender", ((t_chat *)head->data)->sender);
        // cJSON_AddStringToObject(json_node, "recipient", ((t_chat *)head->data)->recipient);
        cJSON_AddStringToObject(json_node, "message", ((t_chat *)head->data)->message);
        cJSON_AddStringToObject(json_node, "timestamp", ((t_chat *)head->data)->timestamp);
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

cJSON *create_json_from_friends_and_chats(t_list *friends, sqlite3 *db, char *username)
{
    int user_id = get_user_id(db, username);
    cJSON *root = cJSON_CreateArray();

    for (t_list *iter = friends; iter != NULL; iter = iter->next)
    {
        cJSON *friend_chat = cJSON_CreateObject();
        cJSON_AddStringToObject(friend_chat, "name", ((t_user *)iter->data)->username);
        if (((t_user *)iter->data)->lastmessage != NULL)
        {
            cJSON_AddStringToObject(friend_chat, "lastmessage", ((t_user *)iter->data)->lastmessage);
        }
        else
        {
            cJSON_AddNullToObject(friend_chat, "lastmessage");
        }
        int friend_id = get_user_id(db, ((t_user *)iter->data)->username);
        t_list *chat_history = get_message_history(db, user_id, friend_id);
        cJSON *messages = cJSON_CreateArray();

        for (t_list *msg_iter = chat_history; msg_iter != NULL; msg_iter = msg_iter->next)
        {
            t_chat *message = (t_chat *)msg_iter->data;

            cJSON *msg_json = cJSON_CreateObject();
            cJSON_AddNumberToObject(msg_json, "message_id", message->id);
            cJSON_AddStringToObject(msg_json, "sender", message->sender);
            cJSON_AddStringToObject(msg_json, "message", message->message);
            cJSON_AddStringToObject(msg_json, "timestamp", message->timestamp);

            cJSON_AddItemToArray(messages, msg_json);
        }

        cJSON_AddItemToObject(friend_chat, "chat_history", messages); // изменено с "messages" на "chat_history"
        cJSON_AddItemToArray(root, friend_chat);

        // Освободить память, занятую списком сообщений
        while (chat_history != NULL)
        {
            t_list *temp = chat_history;
            chat_history = chat_history->next;
            t_chat *message = (t_chat *)temp->data;
            free(message->message);   // освободить память строки message
            free(message->timestamp); // освободить память строки timestamp
            free(temp->data);
            free(temp);
        }
    }

    return root;
}

t_list *extract_group_and_friends_from_json(cJSON *json_object, char **group_name)
{
    *group_name = cJSON_GetObjectItem(json_object, "group_name")->valuestring;

    cJSON *json_friends_array = cJSON_GetObjectItem(json_object, "friends");
    int friends_count = cJSON_GetArraySize(json_friends_array);

    t_list *friends = NULL;

    for (int i = 0; i < friends_count; i++)
    {
        cJSON *json_friend = cJSON_GetArrayItem(json_friends_array, i);
        cJSON *json_friend_username = cJSON_GetObjectItem(json_friend, "username");

        t_user *new_friend = (t_user *)malloc(sizeof(t_user));
        new_friend->username = json_friend_username->valuestring;

        if (new_friend != NULL)
        {
            mx_push_back(&friends, new_friend);
        }
    }

    return friends;
}
