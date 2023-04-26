#include "../inc/server.h"

t_client *create_new_client(const struct sockaddr_in adr, int client_fd) {
    t_client *new_client = (t_client *)malloc(sizeof(t_client));
    new_client->adr = adr;
    new_client->cl_socket = client_fd;
    cli_count++;
    mx_push_back(&users_list, new_client);
    return new_client;
}

