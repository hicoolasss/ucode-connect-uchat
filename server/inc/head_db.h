#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>
#include <strings.h>
#include "../../libs/libmx/inc/libmx.h"
#include <sys/stat.h>
#include <sys/types.h>

#include"sqlite3.h"

int sql_open_db (sqlite3 **db);
void sql_create_db ();
char *join(const char* s1, const char* s2);
void ent_sys(char* login,char* password,SSL *ssl);

//To find one variable
char* sql_get(char* find_var, char* seek_var, char *value_seek_var, int table);

//To update all variables with some rule //change
int sql_update(char *update_var, char *value_update_var, char *seek_var, char *value_seek_var, int table);

//To delete all fields with some rule
int sql_delete(char* seek_var, char *value_seek_var, int table);

//setters //insert
int sql_set_user (char* login, char* password, char* sslkey);

