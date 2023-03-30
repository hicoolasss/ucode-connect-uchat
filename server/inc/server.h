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
    
    char *message;

} t_chat;

typedef struct s_users {
    char *username;
    struct Client *next;
} t_users;

extern t_list *users_list;
extern t_list *user_id;
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
//добавление в друзья по логину
int add_friend(sqlite3 *db, const char *username, const char *friend_username);

/*функции для операций над списками и JSON*/

//односвязный список в JSON
char* serialize_list(t_list* head);
//удаление клиента из списка клиентов
void remove_client(int socket_fd);
//отправка сообщений всем клиентам кроме текущего
void send_message_to_all_clients(char *message, int current_socket, char *login);
//конвертация сообщения в JSON
char *convert_to_json(char *buffer, char *login);
//отправка односвязного списка в клиент
int send_list(SSL *ssl, t_list *head);

/*generate id*/
//generate new user id
int generate_user_id();
//find user by his own id
t_list* find_user(int id);
