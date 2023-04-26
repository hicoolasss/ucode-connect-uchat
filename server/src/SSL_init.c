#include "../inc/server.h"

SSL_CTX *SSL_STX_Init() {

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());

    if (!ctx) {
        // ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_certificate_file(ctx, "server/ssl/certificate.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server/ssl/server_key.key", SSL_FILETYPE_PEM) <= 0 ) {
        // ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        // fprintf(stderr, "Private key does not match the public certificate\n");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void close_connection(SSL *ssl) {
    int socket = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(socket);
}

void close_server(EVP_PKEY *pkey, X509 *x509, SSL_CTX *context) {
    EVP_PKEY_free(pkey);
    X509_free(x509);
    SSL_CTX_free(context);
}

int open_server_connection(int port, struct sockaddr_in *adr, socklen_t adrlen) {
    int serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    int option = 1;
    (*adr).sin_family = AF_INET;
    (*adr).sin_addr.s_addr = htonl(INADDR_ANY);
    (*adr).sin_port = htons(port);
    //(*adr).sin_addr.s_addr = inet_addr("127.0.0.1");

    if(setsockopt(serv_fd, SOL_SOCKET,(SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){ //indus magic
		perror("ERROR: setsockopt failed");
        return EXIT_FAILURE;
	}

    if (bind(serv_fd, (struct sockaddr*)&(*adr), adrlen) < 0 ) {
        perror("ERROR: Socket binding failed\n");
        abort();
    }
    if ( listen(serv_fd, INT_MAX) < 0) {
        perror("ERROR: Can't configure listening port\n");
        abort();
    }

    return serv_fd;
}
