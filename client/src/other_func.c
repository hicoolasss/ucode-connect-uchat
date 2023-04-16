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

    t_list *user_list = NULL;

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
                mx_push_back(&user_list, user);
            }
        }
    }
    cJSON_Delete(json_list);
    return user_list;
}

t_list *receive_list(SSL *ssl)
{
    const int temp_size = 4096;
    char temp[temp_size];

    pthread_mutex_lock(&mutex_recv);
    int bytes_received = SSL_read(ssl, temp, temp_size - 1);
    pthread_mutex_unlock(&mutex_recv);
    if (bytes_received <= 0)
    {
        return NULL;
    }
    if (mx_strcmp("User has no friends", temp) == 0)
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
