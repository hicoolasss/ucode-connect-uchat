#pragma once
#include <stdio.h>
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
#include"sqlite3.h"
#include <openssl/err.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080
#define MAX_CLIENTS 100
#define DB_NAME "server/src/chat.db"
//#include"../../data_base/head_db.h"

typedef struct s_client {
    int serv_fd;

    SSL *ssl;// client ssl structure with coneection to server
    
    struct sockaddr_in adr;
    int cl_socket;

    int id;
    char *login;
    char *passwd;
    bool connected;
} t_client;

typedef struct s_chat {
    
    int id;
    char *sender;
    // char *recipient;
    char *message;
    char *timestamp;

} t_chat;

typedef struct s_user {
    char *username;
    // char *firstname;
    // char *lastname;

    // char *avatarname;
    // char *avatardata;
    // int avatarsize;
} t_user;

extern t_list *users_list;
// extern t_list *user_id;
extern pthread_mutex_t clients_mutex;
extern _Atomic unsigned int cli_count;
t_client *create_new_client(const struct sockaddr_in adr, int client_fd, SSL *ssl);
void *handle_client(void *args);
void free_client(t_client **client, t_list **users_list);
void daemon_server();
SSL_CTX *SSL_STX_Init();
/*функции для операций над базой данных SQL*/

//авторизация
int authenticate_user(sqlite3 *db, const char *username, const char *password);
//регистрация
int register_user(sqlite3 *db, const char *username, const char *password);
//открытие файла бд
sqlite3 *db_open();
//создание таблиц
void sql_create_db();
//получение списка всех пользователей
t_list *get_clients(sqlite3 *db);
//получение списка друзей пользователя
t_list *get_friends(sqlite3 *db, int user_id);
//добавление в друзья по логину
int add_friend(sqlite3 *db, const char *username, const char *friend_username);
//проверка есть ли этот пользователь уже в друзьях или нет
int is_friend(sqlite3 *db, int user_id, int friend_id);
//получает поле айди из поля логина
int get_user_id(sqlite3 *db, const char *login);
//получает поле айди из названия чата
int get_group_id(sqlite3 *db, const char *chatname);
//получение имя из поля айди
char *get_username_by_user_id(sqlite3 *db, int user_id);
//запись чата
int create_chat_record(sqlite3 *db, int chat_id, int sender_id, int recipient_id, const char *message);
//создание нового чата
int sql_create_new_group(sqlite3 *db, const char *chatname, const char *avatarname, const char *avatarblob, int avatarsize);
//запись одиночного чата в бд
int sql_record_message(sqlite3 *db, char *username, char *friendname, const char *message_text);
//запись истории чата в односвязный список
t_list *get_message_history(sqlite3 *db, int user_id, int friend_id);
/*функции для операций над списками и JSON*/

//односвязный список имен в JSON
char* serialize_namelist(t_list* head);
//односвязный список чата в JSON
char* serialize_historylist(t_list* head);
//отправка сообщений всем клиентам кроме текущего
void send_message_to_all_clients(char *message, int current_socket, char *login);
//конвертация сообщения в JSON
char *convert_to_json(char *buffer, char *login);
//отправка односвязного списка имен в клиент
int send_namelist(SSL *ssl, t_list *head);


/*generate id*/
char *generate_uuid();
