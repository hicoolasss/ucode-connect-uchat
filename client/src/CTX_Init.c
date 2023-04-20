#include "../inc/client.h"

SSL_CTX* CTX_initialize_client() { 
    //describes internall ssl library methods (needed for SSL_CTX)
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    return ctx;
}

void open_client_connection(char* server_IP, int port) {
    struct sockaddr_in adr = {0};
    current_client.serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    if(inet_pton(AF_INET, server_IP, &adr.sin_addr)<=0)
    {
        perror("\n inet_pton error occured\n");
        return;
    } 

    while (connect(current_client.serv_fd, (struct sockaddr *)&adr, sizeof(adr)) != 0) {
        close(current_client.serv_fd);
        sleep(1);
        current_client.serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    }

    //inet_pton(AF_INET, server_IP, &adr.sin_addr); //"127.0.0.1"
    current_client.adr = adr;
}

void close_connection(SSL *ssl) {
    int socket = SSL_get_fd(ssl);
    SSL_free(ssl);
    ssl = NULL;
    close(socket);
}

int open_ssl_connection() {
    SSL *ssl;
    SSL_library_init();

    open_client_connection(main_client.ip, main_client.port);
    ssl = SSL_new(main_client.context);
    SSL_set_mode(ssl, SSL_MODE_ASYNC);
    if (SSL_set_fd(ssl, current_client.serv_fd) == 0) {
        perror("ERROR: socket descriptor attachment failed!\n");
        ERR_print_errors_fp(stderr);
        return -1;
    }
    current_client.ssl = ssl;

    if (SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    //printf("SSL: Connected to server with chipher: %s\n", SSL_get_cipher(ssl));
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL) {
        printf("SSL: No certificates configured.\n");
    }
    else {
        X509_free(cert);
    }
    main_client.loaded = true;
    return 0;
}
