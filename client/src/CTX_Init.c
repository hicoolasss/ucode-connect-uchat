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
