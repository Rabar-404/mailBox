#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void tls_handshake() {
    SSL_CTX *config = SSL_CTX_new(TLS_client_method());

    if (config == NULL) {
        ERR_print_errors_fp(stderr);
    }

    SSL *ssl = SSL_new(config);
    
    int result = SSL_do_handshake(ssl);

    SSL_CTX_free(config);
}