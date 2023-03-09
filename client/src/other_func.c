#include "../inc/client.h"

int send_message_to_server(char *buffer)
{
    int len = send_all(cur_client.ssl, buffer, mx_strlen(buffer));

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
    cJSON_AddStringToObject(json, "name", cur_client.login);
    // cJSON_AddStringToObject(json, "password", cur_client.password);
    cJSON_AddStringToObject(json, "message", buffer);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
}

char *registration(int status)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "status", mx_itoa(status));
    cJSON_AddStringToObject(json, "login", cur_client.login);
    cJSON_AddStringToObject(json, "password", cur_client.password);

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
