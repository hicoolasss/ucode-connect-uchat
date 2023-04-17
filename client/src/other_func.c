#include "../inc/client.h"

int send_message_to_server(char *buffer)
{
    int len = send_all(current_client.ssl, buffer, mx_strlen(buffer));
    if (len < 0)
    {
        mx_printstr("Error sending message.\n");
        return 1;
    }
    memset(buffer, 0, mx_strlen(buffer));
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
        if (cJSON_IsString(json_name))
        {
            t_user *user = (t_user *)malloc(sizeof(t_user));
            user->username = mx_strdup(json_name->valuestring);
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
    const int temp_size = 4096;
    char temp[temp_size];

    int bytes_received = SSL_read(ssl, temp, temp_size - 1);
    printf("14523342532452345%s\n",temp);
    if (bytes_received <= 0)
    {
        return NULL;
    }
    // temp[bytes_received] = '\0';
    // printf("%s\n",temp);
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

        t_Friend *new_friend = (t_Friend *)malloc(sizeof(t_Friend)); // Изменено на t_Friend
        new_friend->username = mx_strdup(json_friend_username->valuestring); // Использование strdup для копирования строки
        new_friend->chat_history = NULL; // Инициализация указателя на историю чата

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
                new_chat->message = mx_strdup(json_message_text->valuestring); // Использование strdup для копирования строки
                new_chat->timestamp = mx_strdup(json_message_timestamp->valuestring); // Использование strdup для копирования строки
                new_chat->sender = mx_strdup(json_sender->valuestring); // Использование strdup для копирования строки
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
