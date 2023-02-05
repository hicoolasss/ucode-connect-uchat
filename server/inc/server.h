#define _POSIX_C_SOURCE 200809L // because strdup() -std=c99 rygaetsa
#define _GNU_SOURCE//asprintf

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include "../../libs/libmx/inc/libmx.h"

//db
#include "sqlite3.h"
#include <sys/stat.h>//for stract stat

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define LENGTH 2048

/* Client structure */
typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
} client_t;

void sqlite3_create_db();
//static int callback(void *data, int argc, char **argv, char **azColName);
void *sqlite3_exec_db(char *query, int type);
void update_user_name(char *name, int id);
void update_user_avatar(char *path, int id);
void update_chat_name(char *name, int id);
void update_user_surname(char *surname, int id);
char *get_user_login(int id);
int get_user_id(char *login);
int get_chat_id(char *name);
char  *get_user_avatar(int id);
void db_delete_message(int m_id);



