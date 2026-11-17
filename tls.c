#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int tls_handshake(int sockfd) {
    SSL_CTX *config = SSL_CTX_new(TLS_client_method());

    if (config == NULL) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    SSL_CTX_set_verify(config, SSL_VERIFY_PEER, NULL);
    if (SSL_CTX_set_default_verify_paths(config) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(config);
        return -1;
    }

    SSL *ssl = SSL_new(config);

    if (ssl == NULL) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(config);
        return 1;
    }

    if (SSL_set_fd(ssl, sockfd) != 1) {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(config);
        return 1;
    }

    // TLS handshake
    SSL_set_connect_state(ssl); // set as client
    int result = SSL_do_handshake(ssl);
    if (result != 1) {
        result = SSL_get_error(ssl, result);
        ERR_print_errors_fp(stderr);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(config);

    return 0;
}