#include "../inc/server.h"

t_list *users_list;
FILE *log_file;
pthread_mutex_t clients_mutex;
pthread_mutex_t logs_mutex;
_Atomic unsigned int cli_count;
// t_list *user_id;
// int main(int argc, char **argv)
// {
//     if (argc != 2)
//     {
//         return EXIT_FAILURE;
//     }
//     int port = atoi(argv[1]);

//     struct sockaddr_in cli_addr;

//     // daemon_server();

//     // close(STDIN_FILENO);
//     // close(STDOUT_FILENO);
//     // close(STDERR_FILENO);
//     SSL_CTX *ctx = SSL_STX_Init();

//     socklen_t adr_size = sizeof(cli_addr);

//     int client_fd;
//     struct sockaddr_in adr = {0};
//     int serv_fd = open_server_connection(port, &adr, adr_size);

//     while (1)
//     {
//         // accept client connection
//         client_fd = accept(serv_fd, (struct sockaddr *)&adr, &adr_size);
//         printf("SSL: client accepted\n");
//         // printf("SSL: Connection: %s:%d\n", inet_ntoa(adr.sin_addr), ntohs(adr.sin_port));

//         // handle client
//         pthread_t thread;
//         pthread_mutex_init(&clients_mutex, NULL);
//         t_client *new_client = create_new_client(adr, client_fd);
//         SSL *ssl;
//         ssl = SSL_new(ctx);     // get new SSL state
//         SSL_set_fd(ssl, client_fd); // set connection socket to SSL state

//         if (SSL_accept(ssl) < 0)
//         {
//             ERR_print_errors_fp(stderr);
//             close_connection(ssl);
//             return -1;
//         }

//         new_client->ssl = ssl;
//         pthread_create(&thread, NULL, handle_client, new_client);
//         pthread_join(thread, NULL);
//         sleep(1);
//     }
//     SSL_CTX_free(ctx);
//     close(client_fd);
//     return 0;
// }

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return EXIT_FAILURE;
    }
    daemon_server();
    int port = atoi(argv[1]);
    struct sockaddr_in cli_addr;
    socklen_t adr_size = sizeof(cli_addr);
    int client_fd;
    SSL_CTX *ctx = SSL_STX_Init();
    int server_fd = open_server_connection(port, &cli_addr, adr_size);
    pthread_mutex_init(&clients_mutex, NULL);
    while (1)
    {
        struct sockaddr_in cli_addr;
        socklen_t adr_size = sizeof(cli_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &adr_size);
        // printf("SSL: client accepted\n");

        pthread_t thread;
        t_client *new_client = create_new_client(cli_addr, client_fd);
        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);
        if (SSL_accept(ssl) < 0)
        {
            ERR_print_errors_fp(stderr);
            close_connection(ssl);
            continue;
        }
        new_client->ssl = ssl;

        pthread_create(&thread, NULL, handle_client, new_client);
        pthread_detach(thread);
    }

    SSL_CTX_free(ctx);
    close(server_fd);
    return 0;
}
