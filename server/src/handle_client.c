#include "../inc/server.h"

int recv_all(SSL *sockfd, char *buf, int len);
int send_all(SSL *sockfd, char *buf, int len);
void remove_client(int socket_fd);
void print_message(char *login, char *message);

void *handle_client(void *args)
{
    sql_create_db();
    sqlite3 *db = db_open();
    t_client *current_client = (t_client *)args;
    // char message[2048];
    char buf[2048];
    // main_client.registered = false;
    current_client->connected = false;
    bool is_run = false;
    while (is_run == false)
    {
        memset(buf, 0, sizeof(buf));
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
            char *status = cJSON_GetObjectItemCaseSensitive(json, "status")->valuestring;
            ///////////////////////++++++++++++++++++++++++++++++++++=

            // other func db_regestr_to_serv();
            if (mx_atoi(status) == 0)
            {
                int db_log = authenticate_user(db, login, passwd);
                // int db_log = db_log_to_serv(login, passwd, current_client->ssl);
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
                    current_client->login = mx_strdup(login);
                    mx_printstr(" success\n");
                    memset(login, 0, mx_strlen(login));
                    memset(passwd, 0, mx_strlen(passwd));
                    is_run = true;
                    current_client->connected = true;
                }
            }
            else if (mx_atoi(status) == 1)
            {
                int db_reg = register_user(db, login, passwd);
                if (db_reg == 1)
                {
                    SSL_write(current_client->ssl, "this user already exist\n", 25);
                }
                else if (db_reg == 0)
                {
                    SSL_write(current_client->ssl, "registered\n", 12);
                    mx_printstr(login);
                    mx_printstr(" registered\n");
                    current_client->login = mx_strdup(login);
                    memset(login, 0, mx_strlen(login));
                    memset(passwd, 0, mx_strlen(passwd));
                }
            }
        }
    }
    while (is_run == true)
    {
        memset(buf, 0, sizeof(buf));
        int len = SSL_read(current_client->ssl, buf, sizeof(buf));
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
            break;
        }
        else if (len == 0)
        {
            mx_printstr(current_client->login);
            remove_client(current_client->cl_socket);
            cli_count--;
            printf(" disconnected\n");
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
                is_run = false;
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
