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
    char buf[200000];
    current_client->connected = false;
    bool is_run = false;
    while (1)
    {
        // printf("here\n");
        is_run = false;
        while (is_run == false)
        {
            memset(buf, 0, sizeof(buf));
            int len = SSL_read(current_client->ssl, buf, sizeof(buf) - 1);
            if (len <= 0)
            {
                write_logs("Error: Unable to receive data from server\n");
                remove_client(current_client->cl_socket);
                close(current_client->serv_fd);
                SSL_shutdown(current_client->ssl);
                SSL_free(current_client->ssl);
                cli_count--;
                is_run = false;
                pthread_exit(NULL);
            }
            else
            {
                cJSON *json = cJSON_Parse(buf);
                if (!json)
                {
                    write_logs("Error: Invalid JSON data received from server\n");
                    break;
                }
                // Извлечение данных из JSON-объекта
                char *login = cJSON_GetObjectItemCaseSensitive(json, "login")->valuestring;
                char *passwd = cJSON_GetObjectItemCaseSensitive(json, "password")->valuestring;
                char *status = cJSON_GetObjectItemCaseSensitive(json, "status")->valuestring;

                if (mx_atoi(status) == 0)
                {
                    int db_log = authenticate_user(db, login, passwd);

                    if (db_log == 1)
                    {
                        SSL_write(current_client->ssl, "user not found", 15);
                    }
                    else if (db_log == 2)
                    {
                        SSL_write(current_client->ssl, "incorrect password", 19);
                    }
                    else if (db_log == 0)
                    {
                        current_client->login = mx_strdup(login);
                        char *avatarname = sql_get_image(db, current_client->login);
                        SSL_write(current_client->ssl, avatarname, strlen(avatarname));
                        t_list *current = users_list;
                        is_run = true;
                        current_client->connected = true;
                        while (current != NULL)
                        {
                            if (strcmp(((t_client *)current->data)->login, current_client->login) != 0)
                            {
                                SSL *ssl = ((t_client *)current->data)->ssl;
                                int cmd = SSL_write(ssl, "<Online>", 10);
                                if (cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                                cmd = SSL_write(ssl, current_client->login, mx_strlen(current_client->login));
                                if (cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                            }
                            current = current->next;
                        }
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
                        current_client->login = mx_strdup(login);
                        memset(login, 0, strlen(login));
                        memset(passwd, 0, strlen(passwd));
                    }
                }
                cJSON_Delete(json);
            }
        }
        while (is_run == true)
        {
            memset(buf, 0, sizeof(buf));
            int len = SSL_read(current_client->ssl, buf, sizeof(buf));
            if (len <= 0)
            {
                write_logs("Error: Unable to receive data from server\n");
                remove_client(current_client->cl_socket);
                close(current_client->serv_fd);
                SSL_shutdown(current_client->ssl);
                SSL_free(current_client->ssl);
                cli_count--;
                is_run = false;
                pthread_exit(NULL);
            }
            else
            {
                // Преобразование строки в JSON-объект
                cJSON *json = cJSON_Parse(buf);

                if (!json)
                {
                    write_logs("Error: Invalid JSON data received from server\n");
                    break;
                }

                char *command = cJSON_GetObjectItemCaseSensitive(json, "command")->valuestring;

                if (strcmp(command, "<logout>") == 0)
                {
                    int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                    if (cmd <= 0)
                    {
                        write_json_error(current_client->ssl, cmd);
                    }
                    t_list *current = users_list;
                    while (current != NULL)
                    {
                        if (strcmp(((t_client *)current->data)->login, current_client->login) != 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            cmd = SSL_write(ssl, "<Offline>", 10);
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            cmd = SSL_write(ssl, current_client->login, mx_strlen(current_client->login));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                        }
                        current = current->next;
                    }
                    is_run = false;
                    // break;
                }
                else if (strcmp(command, "<user_list>") == 0)
                {
                    int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                    if (cmd <= 0)
                    {
                        write_json_error(current_client->ssl, cmd);
                    }
                    t_list *clients = get_clients(db);
                    if (clients == NULL)
                    {
                        write_logs("Failed to get clients\n");
                        break;
                    }
                    int result = send_namelist(current_client->ssl, clients);
                    if (result <= 0)
                    {
                        write_json_error(current_client->ssl, cmd);
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
                else if (strcmp(command, "<friend_list>") == 0)
                {
                    int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                    if (cmd <= 0)
                    {
                        write_json_error(current_client->ssl, cmd);
                    }
                    t_list *friends_list = get_friends(db, current_client->login);

                    if (friends_list == NULL)
                    {
                        SSL_write(current_client->ssl, "User has no friends", 20);
                    }
                    else
                    {
                        t_list *current_friend = friends_list;
                        while (current_friend != NULL)
                        {
                            char *username = ((t_user *)current_friend->data)->username;
                            t_list *current_user = users_list;
                            while (current_user != NULL)
                            {
                                if (strcmp(((t_client *)current_user->data)->login, username) == 0)
                                {
                                    ((t_client *)current_user->data)->connected = ((t_user *)current_friend->data)->connected;
                                    break;
                                }
                                current_user = current_user->next;
                            }
                            current_friend = current_friend->next;
                        }
                        cJSON *json = create_json_from_friends_and_chats(friends_list, db, current_client->login);

                        char *json_string = cJSON_Print(json);

                        // Вычислить длину строки
                        int json_string_length = mx_strlen(json_string);

                        // Отправить строку JSON через SSL_write
                        int bytes_written = SSL_write(current_client->ssl, json_string, json_string_length);
                        if (bytes_written <= 0)
                        {
                            // Обработка ошибки
                            write_json_error(current_client->ssl, cmd);
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
                else if (strcmp(command, "<add_friend>") == 0)
                {
                    char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;

                    if (add_friend(db, current_client->login, friendname) == 0)
                    {
                        SSL_write(current_client->ssl, "already your friend", 20);
                    }
                    else
                    {
                        cJSON *json = cJSON_CreateObject();
                        char *avatarname = sql_get_image(db, friendname);
                        cJSON_AddStringToObject(json, "avatarname", avatarname);
                        t_list *current = users_list;
                        while (current != NULL)
                        {
                            if (strcmp(((t_client *)current->data)->login, friendname) == 0)
                            {
                                SSL *ssl = ((t_client *)current->data)->ssl;
                                int cmd = SSL_write(ssl, command, mx_strlen(command));
                                if (cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                                else
                                {
                                    cJSON_AddStringToObject(json, "friendname", current_client->login);
                                    char *json_str = cJSON_Print(json);
                                    SSL_write(ssl, json_str, mx_strlen(json_str));
                                    cJSON_DeleteItemFromObject(json, "friendname");
                                }
                            }
                            if (strcmp(((t_client *)current->data)->login, current_client->login) == 0 && strcmp(((t_client *)current->data)->login, friendname) != 0)
                            {
                                SSL *ssl = ((t_client *)current->data)->ssl;
                                int cmd = SSL_write(ssl, command, mx_strlen(command));
                                if (cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                                else
                                {
                                    cJSON_AddStringToObject(json, "friendname", friendname);
                                    char *json_str = cJSON_Print(json);
                                    SSL_write(ssl, json_str, mx_strlen(json_str));
                                    cJSON_DeleteItemFromObject(json, "friendname");
                                }
                            }
                            current = current->next;
                        }
                        cJSON_Delete(json);
                    }
                }
                else if (strcmp(command, "<send_message_in_chat>") == 0)
                {
                    char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;
                    char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
                    t_chat *message_data = sql_record_message(db, current_client->login, friendname, message);
                    cJSON *json_message = cJSON_CreateObject();
                    cJSON_AddStringToObject(json_message, "sender", message_data->sender);
                    cJSON_AddStringToObject(json_message, "message", message_data->message);
                    cJSON_AddStringToObject(json_message, "timestamp", message_data->timestamp);
                    cJSON_AddNumberToObject(json_message, "id", message_data->id);
                    t_list *current = users_list;

                    while (current != NULL)
                    {
                        if (strcmp(((t_client *)current->data)->login, friendname) == 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_ssl_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", current_client->login);
                                char *json_str = cJSON_Print(json_message);
                                int json_cmd = SSL_write(ssl, json_str, mx_strlen(json_str));
                                if (json_cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        else if (strcmp(((t_client *)current->data)->login, current_client->login) == 0 && strcmp(((t_client *)current->data)->login, friendname) != 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", friendname);
                                char *json_str = cJSON_Print(json_message);
                                int json_cmd = SSL_write(ssl, json_str, mx_strlen(json_str));
                                if (json_cmd <= 0)
                                {
                                    write_json_error(ssl, cmd);
                                }
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        current = current->next;
                    }

                    cJSON_Delete(json_message);
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
                else if (strcmp(command, "<delete_message_in_chat>") == 0)
                {
                    char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
                    char *username = cJSON_GetObjectItemCaseSensitive(json, "sender")->valuestring;
                    char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;
                    int message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id")->valueint;

                    if (sql_delete_message_from_dialog(db, message_id, username, message) == -1)
                    {
                        write_logs("Ошибка выполнения SQL\n");
                        continue;
                    }

                    cJSON *json_message = cJSON_CreateObject();
                    cJSON_AddStringToObject(json_message, "message", message);
                    cJSON_AddNumberToObject(json_message, "message_id", message_id);

                    t_list *current = users_list;
                    while (current != NULL)
                    {
                        if (strcmp(((t_client *)current->data)->login, friendname) == 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", current_client->login);
                                char *json_str = cJSON_Print(json_message);
                                SSL_write(ssl, json_str, mx_strlen(json_str));
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        if (strcmp(((t_client *)current->data)->login, current_client->login) == 0 && strcmp(((t_client *)current->data)->login, friendname) != 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", friendname);
                                char *json_str = cJSON_Print(json_message);
                                SSL_write(ssl, json_str, mx_strlen(json_str));
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        current = current->next;
                    }
                    cJSON_Delete(json_message);
                }
                else if (strcmp(command, "<update_message_in_chat>") == 0)
                {
                    char *new_message = cJSON_GetObjectItemCaseSensitive(json, "new_message")->valuestring;
                    char *old_message = cJSON_GetObjectItemCaseSensitive(json, "old_message")->valuestring;
                    char *username = cJSON_GetObjectItemCaseSensitive(json, "sender")->valuestring;
                    char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friendname")->valuestring;
                    int message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id")->valueint;

                    if (sql_update_message_in_dialog(db, message_id, old_message, new_message, username) == -1)
                    {
                        write_logs("Ошибка выполнения SQL\n");
                        continue;
                    }

                    cJSON *json_message = cJSON_CreateObject();
                    cJSON_AddStringToObject(json_message, "new_message", new_message);
                    cJSON_AddNumberToObject(json_message, "message_id", message_id);

                    t_list *current = users_list;
                    while (current != NULL)
                    {
                        if (strcmp(((t_client *)current->data)->login, friendname) == 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", current_client->login);
                                char *json_str = cJSON_Print(json_message);
                                SSL_write(ssl, json_str, mx_strlen(json_str));
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        if (strcmp(((t_client *)current->data)->login, current_client->login) == 0 && strcmp(((t_client *)current->data)->login, friendname) != 0)
                        {
                            SSL *ssl = ((t_client *)current->data)->ssl;
                            int cmd = SSL_write(ssl, command, mx_strlen(command));
                            if (cmd <= 0)
                            {
                                write_json_error(ssl, cmd);
                            }
                            else
                            {
                                cJSON_AddStringToObject(json_message, "friendname", friendname);
                                char *json_str = cJSON_Print(json_message);
                                SSL_write(ssl, json_str, mx_strlen(json_str));
                                cJSON_DeleteItemFromObject(json_message, "friendname");
                            }
                        }
                        current = current->next;
                    }
                    cJSON_Delete(json_message);
                }
                else if (strcmp(command, "<update_image>") == 0)
                {
                    char *filename = cJSON_GetObjectItemCaseSensitive(json, "filename")->valuestring;
                    if (save_image_to_db(db, current_client->login, filename) != 0)
                    {
                        int cmd = SSL_write(current_client->ssl, "Avatar not saved", 17);
                        if (cmd <= 0)
                        {
                            write_ssl_error(current_client->ssl, cmd);
                        }
                    }
                    else
                    {
                        cJSON *json_message = cJSON_CreateObject();
                        cJSON_AddStringToObject(json_message, "username", current_client->login);
                        cJSON_AddStringToObject(json_message, "avatarname", filename);
                        char *json_str = cJSON_Print(json_message);

                        t_list *current = users_list;
                        while (current != NULL)
                        {
                            if (((t_client *)current->data)->cl_socket != current_client->cl_socket)
                            {
                                SSL *ssl = ((t_client *)current->data)->ssl;
                                int cmd = SSL_write(ssl, command, mx_strlen(command));
                                if (cmd <= 0)
                                {
                                    {
                                        write_json_error(ssl, cmd);
                                    }
                                }
                                SSL_write(ssl, json_str, mx_strlen(json_str));
                            }
                            current = current->next;
                        }
                        cJSON_Delete(json_message);
                    }
                }
                cJSON_Delete(json);
            }
        }
    }
    return NULL;
}
