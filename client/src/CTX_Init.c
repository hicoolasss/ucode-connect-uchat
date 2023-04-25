#include "../inc/client.h"

SSL_CTX *CTX_initialize_client()
{
    // describes internall ssl library methods (needed for SSL_CTX)
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    return ctx;
}

void open_client_connection(char *server_IP, int port)
{
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&current_client.adr, 0, sizeof(current_client.adr));
    current_client.adr.sin_family = AF_INET;
    current_client.adr.sin_port = htons(port);
    current_client.adr.sin_addr.s_addr = inet_addr(server_IP);

    // while (connect(current_client.serv_fd, (struct sockaddr *)&current_client.adr, sizeof(current_client.adr)) != 0) {
    //     close(current_client.serv_fd);
    //     sleep(1);
    //     current_client.serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    // }
    if (connect(serv_fd, (struct sockaddr *)&current_client.adr, sizeof(current_client.adr)) < 0)
    {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }
    current_client.serv_fd = serv_fd;
}

void close_connection(SSL *ssl)
{
    int socket = SSL_get_fd(ssl);
    SSL_free(ssl);
    ssl = NULL;
    close(socket);
}

int open_ssl_connection()
{
    // SSL *ssl;
    SSL_library_init();

    open_client_connection(main_client.ip, main_client.port);
    current_client.ssl = SSL_new(main_client.context);
    SSL_set_mode(current_client.ssl, SSL_MODE_ASYNC);
    if (SSL_set_fd(current_client.ssl, current_client.serv_fd) == 0)
    {
        perror("ERROR: socket descriptor attachment failed!\n");
        ERR_print_errors_fp(stderr);
        return -1;
    }
    // current_client.ssl = ssl;

    if (SSL_connect(current_client.ssl) == -1)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    // printf("SSL: Connected to server with chipher: %s\n", SSL_get_cipher(ssl));
    //  X509 *cert = SSL_get_peer_certificate(ssl);
    //  if (cert == NULL) {
    //      printf("SSL: No certificates configured.\n");
    //  }
    //  else {
    //      X509_free(cert);
    //  }
    main_client.loaded = true;
    return 0;
}
