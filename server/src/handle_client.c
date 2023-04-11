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
                char *login = cJSON_GetObjectItemCaseSensitive(json, "login")->valuestring;
                print_message(login, "logout\n");
                remove_client(current_client->cl_socket);
                cli_count--;
                SSL_write(current_client->ssl, "success\n", 9);
                is_run = false;
            }
            else if (mx_strcmp(command, "<user_list>") == 0)
            {
                t_list *clients = get_clients(db);

                if (clients == NULL)
                {
                    fprintf(stderr, "Failed to get clients\n");
                    break;
                }

                // t_list *ccc = clients;

                // int command = SSL_write(current_client->ssl, "<user_list>", 12);
                // if (command < 0)
                // {
                //     printf("I can't send command to %s\n, check his connection", current_client->login);
                // }
                // else
                // {
                //     printf("Success sending command to %s\n", current_client->login);
                // }

                // while (ccc != NULL)
                // {
                //     printf("Username: %s\n", ((t_user *)ccc->data)->username);
                //     ccc = ccc->next;
                // }

                int result = send_namelist(current_client->ssl, clients);
                if (result > 0)
                {
                    printf("List sent successfully to %s\n", current_client->login);
                }
                else
                {
                    printf("Error sending list to %s\n", current_client->login);
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
                // int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                // if (cmd < 0)
                // {
                //     printf("I can't send command to %s\n, check his connection", current_client->login);
                // }
                // else
                // {
                //     printf("Success sending command to %s\n", current_client->login);
                // }
                int user_id = get_user_id(db, current_client->login);
                t_list *friends_list = get_friends(db, user_id);

                if (friends_list == NULL)
                    printf("User has no friends.\n");

                int result = send_namelist(current_client->ssl, friends_list);
                if (result > 0)
                {
                    printf("List sent successfully to %s\n", current_client->login);
                }
                else
                {
                    printf("Error sending list to %s\n", current_client->login);
                }
                while (friends_list != NULL)
                {
                    t_list *temp = friends_list;
                    friends_list = friends_list->next;
                    free(temp->data);
                    free(temp);
                }
            }
            else if (mx_strcmp(command, "<add_friend>") == 0)
            {
                char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;

                mx_printchar('\n');
                if (len < 0)
                {
                    mx_printstr("Error: Unable to receive data from server\n");
                    break;
                }

                if (add_friend(db, current_client->login, friendname) == 0)
                {
                    mx_printstr("User not found, write friend's login correct");
                    // SSL_write(current_client->ssl, "User not found, write friend's login correct", 45);
                }
                // else
                // {
                //     SSL_write(current_client->ssl, "friend add", 11);
                // }
                // cJSON_Delete(cjson_add_friend);
            }
            else if (mx_strcmp(command, "<send_message_to>") == 0)
            {
                // int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                // if (cmd < 0)
                // {
                //     printf("I can't send command to %s\n, check his connection", current_client->login);
                // }
                // else
                // {
                //     printf("Success sending command to %s\n", current_client->login);
                // }
                // char *sender = cJSON_GetObjectItemCaseSensitive(json, "sender")->valuestring;
                char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;
                char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;

                if(sql_record_message(db, current_client->login, friendname, message) == 0) {
                    mx_printstr("Success recording\n");
                }
                // char *json_str = convert_to_json(message, current_client->login);
                // t_list *current = users_list;
                // while (current != NULL)
                // {
                //     // if (((t_client *)current->data)->cl_socket != current_socket && ((t_client *)current->data)->connected == true)
                //     if (((t_client *)current->data)->login != friendname && ((t_client *)current->data)->connected == true)
                //     {
                //         SSL *ssl = ((t_client *)current->data)->ssl;
                //         SSL_write(ssl, "recv_message_from", 18);
                //         SSL_write(ssl, json_str, mx_strlen(json_str));
                //     }
                //     current = current->next;
                // }
            }
            else if (mx_strcmp(command, "<show_history>") == 0)
            {
                // int cmd = SSL_write(current_client->ssl, command, mx_strlen(command));
                // if (cmd < 0)
                // {
                //     printf("I can't send command to %s\n, check his connection", current_client->login);
                // }
                // else
                // {
                //     printf("Success sending command to %s\n", current_client->login);
                // }
                char *friendname = cJSON_GetObjectItemCaseSensitive(json, "friend")->valuestring;
                int user_id = get_user_id(db, current_client->login);
                int friend_id = get_user_id(db, friendname);

                t_list *chat_history = get_message_history(db, user_id, friend_id);

                if (chat_history == NULL)
                {
                    printf("chat empty\n");
                    SSL_write(current_client->ssl, "chat empty", 11);
                    continue;
                }
                char *serialized_list = serialize_historylist(chat_history);
                int result = SSL_write(current_client->ssl, serialized_list, strlen(serialized_list));
                free(serialized_list);

                if (result > 0)
                {
                    printf("List sent successfully to %s\n", current_client->login);
                }
                else
                {
                    printf("Error sending list to %s\n", current_client->login);
                }

                while (chat_history != NULL)
                {
                    t_list *temp = chat_history;
                    chat_history = chat_history->next;
                    free(temp->data);
                    free(temp);
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
