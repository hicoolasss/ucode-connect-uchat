#pragma once
#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <strings.h>
#include <stdatomic.h>
#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/openssl/openssl/ssl.h"
#include "sqlite3.h"
#include <openssl/err.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080
#define MAX_CLIENTS 100
#define DB_NAME "server/src/chat.db"
// #include"../../data_base/head_db.h"

typedef struct s_client
{
    int serv_fd;

    SSL *ssl; // client ssl structure with coneection to server

    struct sockaddr_in adr;
    int cl_socket;

    int id;
    char *login;
    char *passwd;
    bool connected;
} t_client;

typedef struct s_chat
{

    int id;
    char *sender;
    char *message;
    char *timestamp;

} t_chat;

typedef struct s_user
{
    char *username;
    char *avatarname;
    char *lastmessage;
} t_user;

extern t_list *users_list;
extern pthread_mutex_t clients_mutex;
extern pthread_mutex_t logs_mutex;
extern _Atomic unsigned int cli_count;
t_client *create_new_client(const struct sockaddr_in adr, int client_fd, SSL *ssl);
void *handle_client(void *args);
void free_client(t_client **client, t_list **users_list);
void daemon_server();
void write_logs(char *message);
SSL_CTX *SSL_STX_Init();
unsigned char *base64_decode(const char *input, size_t *out_length);
/*функции для операций над базой данных SQL*/

// авторизация
int authenticate_user(sqlite3 *db, const char *username, const char *password);
// регистрация
int register_user(sqlite3 *db, const char *username, const char *password);
// открытие файла бд
sqlite3 *db_open();
// создание таблиц
void sql_create_db();
// получение списка всех пользователей
t_list *get_clients(sqlite3 *db);
// получение списка друзей пользователя
t_list *get_friends(sqlite3 *db, const char *username);
// добавление в друзья по логину
int add_friend(sqlite3 *db, const char *username, const char *friend_username);
// проверка есть ли этот пользователь уже в друзьях или нет
int is_friend(sqlite3 *db, int user_id, int friend_id);
// получает поле айди из поля логина
int get_user_id(sqlite3 *db, const char *login);
// получает поле айди из названия чата
int get_group_id(sqlite3 *db, const char *chatname);
// получение имя из поля айди
char *get_username_by_user_id(sqlite3 *db, int user_id);
// запись одиночного чата в бд
t_chat *sql_record_message(sqlite3 *db, char *username, char *friendname, const char *message_text);
// запись истории чата в односвязный список
t_list *get_message_history(sqlite3 *db, int user_id, int friend_id);
// вывод последнего сообщения в чате с этим человеком
char *get_last_message_from_dialog(sqlite3 *db, const char *username, const char *friendname);
// достает из json название групы и список учасников групы
t_list *extract_group_and_friends_from_json(cJSON *json_object, char **group_name);
// удаляет сообщение из чата по его айди
int sql_delete_message_from_dialog(sqlite3 *db, int message_id, const char *username, const char *message);
// изменяет сообщение в чате по его айди
int sql_update_message_in_dialog(sqlite3 *db, int message_id, const char *old_message, const char *new_message, const char *username);
// сохранение аватара в базе данных
int save_image_to_db(sqlite3 *db, const char *username, const char *filename);
// получение аватара пользователя
char *sql_get_image(sqlite3 *db, const char *username);
/*функции для операций над списками и JSON*/

// односвязный список имен в JSON
char *serialize_namelist(t_list *head);
// односвязный список чата в JSON
char *serialize_historylist(t_list *head);
// отправка сообщений всем клиентам кроме текущего
void send_message_to_all_clients(char *message, int current_socket, char *login);
// конвертация сообщения в JSON
char *convert_to_json(char *buffer, char *login);
// отправка односвязного списка имен в клиент
int send_namelist(SSL *ssl, t_list *head);
// список друзей с их историей чата
cJSON *create_json_from_friends_and_chats(t_list *friends, sqlite3 *db, char *username);
