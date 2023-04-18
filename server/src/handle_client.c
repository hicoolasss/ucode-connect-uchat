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
            cJSON_Delete(json);
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
            mx_printstr(buf);
            mx_printchar('\n');
            cJSON *json = cJSON_Parse(buf);

            if (!json)
            {
                printf("Error: Invalid JSON data received from server\n");
                break;
            }

            char *command = cJSON_GetObjectItemCaseSensitive(json, "command")->valuestring;

            if (mx_strcmp(command, "<logout>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                if (cmd <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, cmd);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }
                char *login = cJSON_GetObjectItemCaseSensitive(json, "login")->valuestring;
                print_message(login, "logout\n");
                remove_client(current_client->cl_socket);
                cli_count--;
                is_run = false;
            }
            else if (mx_strcmp(command, "<user_list>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                if (cmd <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, cmd);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }
                t_list *clients = get_clients(db);
                if (clients == NULL)
                {
                    fprintf(stderr, "Failed to get clients\n");
                    break;
                }

                int result = send_namelist(current_client->ssl, clients);
                if (result <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, result);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }
                // Освобождаем память, выделенную для списка
                while (clients != NULL)
                {
                    t_list *temp = clients;
                    clients = clients->next;
                    free(temp->data);
                    free(temp);
                }
            }
            else if (mx_strcmp(command, "<friend_list>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                if (cmd <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, cmd);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }
                t_list *friends_list = get_friends(db, current_client->login);

                if (friends_list == NULL)
                {
                    printf("User has no friends.\n");
                    SSL_write(current_client->ssl, "User has no friends", 20);
                }
                else
                {
                    cJSON *json = create_json_from_friends_and_chats(friends_list, db, current_client->login);

                    char *json_string = cJSON_Print(json);

                    // Вычислить длину строки
                    int json_string_length = mx_strlen(json_string);

                    // Отправить строку JSON через SSL_write
                    int bytes_written = SSL_write(current_client->ssl, json_string, json_string_length);
                    if (bytes_written <= 0)
                    {
                        // Обработка ошибки
                        int error_code = SSL_get_error(current_client->ssl, bytes_written);
                        fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                    }
                    // Освободить память, выделенную для строки JSON
                    cJSON_free(json_string);

                    while (friends_list != NULL)
                    {
                        t_list *temp = friends_list;
                        friends_list = friends_list->next;
                        free(temp->data);
                        free(temp);
                    }
                }
            }
            else if (mx_strcmp(command, "<add_friend>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                if (cmd <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, cmd);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }

                char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;

                if (len < 0)
                {
                    mx_printstr("Error: Unable to receive data from server\n");
                    break;
                }
                if (add_friend(db, current_client->login, friendname) == 0)
                {
                    mx_printstr("already your friend\n");
                    SSL_write(current_client->ssl, "already your friend", 20);
                    break;
                }
                else
                {
                    cJSON *json = cJSON_CreateObject();
                    cJSON_AddStringToObject(json, "friendname", friendname);
                    char *json_str = cJSON_Print(json);
                    cJSON_Delete(json);

                    t_list *current = users_list;
                    while (current != NULL)
                    {
                        // if (((t_client *)current->data)->cl_socket != current_socket && ((t_client *)current->data)->connected == true)
                        if (((t_client *)current->data)->login == friendname || ((t_client *)current->data)->login == current_client->login)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            SSL_write(ssl, json_str, mx_strlen(json_str));
                        }
                        // else mx_printstr("null\n");

                        current = current->next;
                    }

                    // SSL_write(current_client->ssl, json_str, mx_strlen(json_str));
                }
            }
            else if (mx_strcmp(command, "<send_message>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                // if (cmd <= 0)
                // {
                //     int error_code = SSL_get_error(current_client->ssl, cmd);
                //     fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                // }
                char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;
                char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
                t_chat *message_data = sql_record_message(db, current_client->login, friendname, message);
                cJSON *json = cJSON_CreateObject();
                cJSON_AddStringToObject(json, "sender", message_data->sender);
                cJSON_AddStringToObject(json, "message", message_data->message);
                cJSON_AddStringToObject(json, "timestamp", message_data->timestamp);
                cJSON_AddNumberToObject(json, "id", message_data->id);
                // cJSON_AddStringToObject(json, "friendname", friendname);
                t_list *current = users_list;

                while (current != NULL)
                {
                    if (strcmp(((t_client *)current->data)->login, friendname) == 0)
                    {
                        int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                        if (cmd <= 0)
                        {
                            int error_code = SSL_get_error(current_client->ssl, cmd);
                            fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                        }
                        cJSON_AddStringToObject(json, "friendname", current_client->login);
                        char *json_str = cJSON_Print(json);
                        SSL *ssl = ((t_client *)current->data)->ssl;
                        SSL_write(ssl, json_str, mx_strlen(json_str));
                        cJSON_DeleteItemFromObject(json, "friendname");
                    }
                    if (strcmp(((t_client *)current->data)->login, current_client->login) == 0 && strcmp(((t_client *)current->data)->login, friendname) != 0)
                    {
                        int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                        if (cmd <= 0)
                        {
                            int error_code = SSL_get_error(current_client->ssl, cmd);
                            fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                        }
                        cJSON_AddStringToObject(json, "friendname", friendname);
                        char *json_str = cJSON_Print(json);
                        SSL *ssl = ((t_client *)current->data)->ssl;
                        SSL_write(ssl, json_str, mx_strlen(json_str));
                        cJSON_DeleteItemFromObject(json, "friendname");
                    }
                    current = current->next;
                }

                cJSON_Delete(json);
                if (message_data)
                {
                    if (message_data->message)
                    {
                        free(message_data->message);
                    }
                    if (message_data->timestamp)
                    {
                        free(message_data->timestamp);
                    }
                    if (message_data->sender)
                    {
                        free(message_data->sender);
                    }
                    free(message_data);
                }
            }
            else if (mx_strcmp(command, "<create_group>") == 0)
            {
                int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                if (cmd <= 0)
                {
                    int error_code = SSL_get_error(current_client->ssl, cmd);
                    fprintf(stderr, "Error sending JSON string: %s\n", ERR_error_string(error_code, NULL));
                }
            }
            cJSON_Delete(json);
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
