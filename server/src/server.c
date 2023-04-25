#include "../inc/server.h"

t_list *users_list;
FILE *log_file;
pthread_mutex_t clients_mutex;
pthread_mutex_t logs_mutex;
_Atomic unsigned int cli_count;
// t_list *user_id;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        // printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);

    // daemon_server();

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);
    SSL_CTX *ctx = SSL_STX_Init();

    // int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_fd < 0)
    // {
    //     write_logs("Failed to create socket\n");
    //     exit(EXIT_FAILURE);
    // }

    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(port);
    // server_addr.sin_addr.s_addr = INADDR_ANY;

    // if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    // {
    //     write_logs("Failed to bind to address\n");
    //     exit(EXIT_FAILURE);
    // }

    // if (listen(server_fd, MAX_CLIENTS) < 0)
    // {
    //     write_logs("Failed to listen on socket\n");
    //     exit(EXIT_FAILURE);
    // }
    socklen_t adr_size = sizeof(cli_addr);

    int client_id = 0;
    int client_fd;
    struct sockaddr_in adr = {0};
    int serv_fd = open_server_connection(port, &adr, adr_size);

    while (1)
    {
        // accept client connection
        client_fd = accept(serv_fd, (struct sockaddr *)&adr, &adr_size);
        printf("SSL: client accepted\n");
        // printf("SSL: Connection: %s:%d\n", inet_ntoa(adr.sin_addr), ntohs(adr.sin_port));

        // handle client
        pthread_t thread;
        pthread_mutex_init(&clients_mutex, NULL);
        t_client *new_client = create_new_client(adr, client_fd);
        SSL *ssl;
        ssl = SSL_new(ctx);     // get new SSL state
        SSL_set_fd(ssl, client_fd); // set connection socket to SSL state

        if (SSL_accept(ssl) < 0)
        {
            ERR_print_errors_fp(stderr);
            close_connection(ssl);
            return -1;
        }

        new_client->ssl = ssl;
        pthread_create(&thread, NULL, handle_client, new_client);
        pthread_join(thread, NULL);
        sleep(1);
    }
    // while (1)
    // {
    //     SSL *ssl;

    //     int client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &adr_size);

    //     ssl = SSL_new(ctx);
    //     SSL_set_fd(ssl, client_fd);
    //     SSL_accept(ssl);

    //     pthread_t thread;
    //     pthread_mutex_init(&clients_mutex, NULL);
    //     t_client *new_client = create_new_client(cli_addr, client_fd, ssl);
    //     pthread_create(&thread, NULL, handle_client, new_client);
    //     pthread_join(thread, NULL);
    // }
    // SSL_CTX_free(ctx);
    // close(server_fd);
    pthread_exit(NULL);
    return 0;
}
