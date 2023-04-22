#include "../inc/client.h"

int send_message_to_server(char *buffer)
{
    int len = stable_sending(current_client.ssl, buffer, mx_strlen(buffer));
    if (len < 0)
    {
        mx_printstr("Error sending message.\n");
        return 1;
    }
    return 0;
}

char *convert_to_json(char *buffer)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", current_client.login);
    // cJSON_AddStringToObject(json, "password", current_client.password);
    cJSON_AddStringToObject(json, "message", buffer);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
}

char *registration(int status)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "status", mx_itoa(status));
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "password", current_client.password);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
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

t_list *deserialize_name_list(const char *json_str)
{
    cJSON *json_list = cJSON_Parse(json_str);
    if (json_list == NULL)
    {
        return NULL;
    }

    t_list *user_list_temp = NULL;

    cJSON *json_node = NULL;
    cJSON_ArrayForEach(json_node, json_list)
    {
        cJSON *json_name = cJSON_GetObjectItem(json_node, "name");
        // cJSON *json_avatarname = cJSON_GetObjectItem(json_node, "avatarname");

        if (cJSON_IsString(json_name))
        {
            t_user *user = (t_user *)malloc(sizeof(t_user));
            user->username = mx_strdup(json_name->valuestring);
            // user->avatarname = mx_strdup(json_avatarname->valuestring);
            if (user != NULL)
            {
                mx_push_back(&user_list_temp, user);
            }
        }
    }
    cJSON_Delete(json_list);
    return user_list_temp;
}

t_list *receive_list(SSL *ssl)
{
    char temp[2000000];

    int bytes_received = stable_recv(ssl, temp, sizeof(temp));
    if (bytes_received <= 0)
    {
        return NULL;
    }
    temp[bytes_received] = '\0';
    return deserialize_name_list(temp);
}

t_list *deserialize_chathistory_list(const char *json_str)
{
    cJSON *json_list = cJSON_Parse(json_str);
    if (json_list == NULL)
    {
        return NULL;
    }

    t_list *chat_history = NULL;

    cJSON *json_node = NULL;
    cJSON_ArrayForEach(json_node, json_list)
    {
        int message_id = cJSON_GetObjectItem(json_node, "message_id")->valueint;
        char *sender = cJSON_GetObjectItem(json_node, "sender")->valuestring;
        char *message = cJSON_GetObjectItem(json_node, "message")->valuestring;
        char *timestamp = cJSON_GetObjectItem(json_node, "timestamp")->valuestring;

        t_chat *chat = (t_chat *)malloc(sizeof(t_chat));
        chat->id = message_id;
        chat->sender = mx_strdup(sender);
        chat->message = mx_strdup(message);
        chat->timestamp = mx_strdup(timestamp);

        if (chat != NULL)
        {
            mx_push_back(&chat_history, chat);
        }
    }
    cJSON_Delete(json_list);
    return chat_history;
}

t_list *process_json_object(cJSON *json_object)
{
    cJSON *json_friend_list = json_object; // Изменено, чтобы использовать входной объект напрямую

    int friend_count = cJSON_GetArraySize(json_friend_list);

    t_list *friend = NULL;

    for (int i = 0; i < friend_count; i++)
    {
        cJSON *json_friend = cJSON_GetArrayItem(json_friend_list, i);
        cJSON *json_friend_username = cJSON_GetObjectItem(json_friend, "name");
        cJSON *json_lastmessage = cJSON_GetObjectItem(json_friend, "lastmessage");

        t_Friend *new_friend = (t_Friend *)malloc(sizeof(t_Friend));         // Изменено на t_Friend
        new_friend->username = mx_strdup(json_friend_username->valuestring); // Использование strdup для копирования строки
        if (json_lastmessage->valuestring != NULL)
        {
            new_friend->lastmessage = mx_strdup(json_lastmessage->valuestring);
        }
        else new_friend->lastmessage = "Nothing here...";
        new_friend->in_chat = false;
        new_friend->chat_history = NULL; // Инициализация указателя на историю чата
        new_friend->in_chat = false;
        if (new_friend != NULL)
        {
            mx_push_back(&friend, new_friend);
        }

        cJSON *json_chat_history = cJSON_GetObjectItem(json_friend, "chat_history");
        int chat_history_count = cJSON_GetArraySize(json_chat_history);

        if (chat_history_count > 0)
        {
            t_list *chat_history = NULL;

            for (int j = 0; j < chat_history_count; j++)
            {
                cJSON *json_message = cJSON_GetArrayItem(json_chat_history, j);
                cJSON *json_message_id = cJSON_GetObjectItem(json_message, "message_id");
                cJSON *json_message_text = cJSON_GetObjectItem(json_message, "message");
                cJSON *json_message_timestamp = cJSON_GetObjectItem(json_message, "timestamp");
                cJSON *json_sender = cJSON_GetObjectItem(json_message, "sender");

                t_chat *new_chat = (t_chat *)malloc(sizeof(t_chat));
                new_chat->id = json_message_id->valueint;
                new_chat->message = mx_strdup(json_message_text->valuestring);        // Использование strdup для копирования строки
                new_chat->timestamp = mx_strdup(json_message_timestamp->valuestring); // Использование strdup для копирования строки
                new_chat->sender = mx_strdup(json_sender->valuestring);               // Использование strdup для копирования строки
                if (new_chat != NULL)
                {
                    mx_push_back(&chat_history, new_chat);
                }
            }
            new_friend->chat_history = chat_history; // Изменено на new_friend
        }
    }
    return friend;
}

cJSON *create_json_from_group_and_friends(const char *group_name, t_list *friends)
{
    cJSON *group_json = cJSON_CreateObject();
    cJSON_AddStringToObject(group_json, "group_name", group_name);

    cJSON *friends_array = cJSON_CreateArray();

    for (t_list *iter = friends; iter != NULL; iter = iter->next)
    {
        cJSON *friend_json = cJSON_CreateObject();
        cJSON_AddStringToObject(friend_json, "username", ((t_user *)iter->data)->username);

        cJSON_AddItemToArray(friends_array, friend_json);
    }

    cJSON_AddItemToObject(group_json, "friends", friends_array);

    return group_json;
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

int stable_sending(SSL *ssl, void *buf, int size)
{
    int receive = 0;

    if (main_client.loaded)
    {
        while ((receive = send_all(ssl, buf, size)) < 0)
        {
            main_client.loaded = false;
            close_connection(current_client.ssl);
            printf("Trying to reconnect\n");
            open_ssl_connection();
            ssl = current_client.ssl;
            // Отправка данных для авторизации на сервер
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "status", "0");
            cJSON_AddStringToObject(json, "login", current_client.login);
            cJSON_AddStringToObject(json, "password", current_client.password);

            char *json_str = cJSON_Print(json);
            send_all(ssl, json_str, mx_strlen(json_str));
            cJSON_Delete(json);
        }
    }
    return receive;
}

int stable_recv(SSL *ssl, void *buf, int size)
{
    int receive = 0;
    if (main_client.loaded)
    {
        while ((receive = SSL_read(ssl, buf, size)) <= 0)
        {
            main_client.loaded = false;
            printf("Trying to reconnect\n");
            close_connection(current_client.ssl);
            open_ssl_connection();
            ssl = current_client.ssl;
            // Отправка данных для авторизации на сервер
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "status", "0");
            cJSON_AddStringToObject(json, "login", current_client.login);
            cJSON_AddStringToObject(json, "password", current_client.password);

            char *json_str = cJSON_Print(json);
            send_all(ssl, json_str, mx_strlen(json_str));
            cJSON_Delete(json);
        }
    }
    return receive;
}

unsigned char *base64_decode(const char *input, int *out_length)
{
    BIO *b64, *bmem;
    size_t length = strlen(input);

    unsigned char *buffer = (unsigned char *)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    *out_length = BIO_read(bmem, buffer, length);

    BIO_free_all(bmem);

    return buffer;
}
