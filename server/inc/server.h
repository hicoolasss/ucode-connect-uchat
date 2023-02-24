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
#define PORT 8080
#define MAX_CLIENTS 5

typedef struct s_client {
    int serv_fd;

    SSL *ssl;// client ssl structure with coneection to server
    
    struct sockaddr_in adr;
    int cl_socket;

    int id;
    char *login;
    char *passwd;

} t_client;


typedef struct s_chat {
    
    char *message;

} t_chat;

extern t_list *users_list;
extern pthread_mutex_t clients_mutex;
extern _Atomic unsigned int cli_count;
t_client *create_new_client(const struct sockaddr_in adr, int client_fd, SSL *ssl);
void *handle_client(void *args);
void free_client(t_client **client, t_list **users_list);
void daemon_server();
SSL_CTX *SSL_STX_Init();
