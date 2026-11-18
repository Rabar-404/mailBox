#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/socket.h>
#include <curl/curl.h>

SSL *tls_handshake(int sockfd) {
    SSL_CTX *config = SSL_CTX_new(TLS_client_method());

    if (config == NULL) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(config);
        return NULL;
    }
    
    SSL_CTX_set_verify(config, SSL_VERIFY_PEER, NULL);
    if (SSL_CTX_set_default_verify_paths(config) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(config);
        return NULL;
    }

    SSL *ssl = SSL_new(config);

    if (ssl == NULL) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(config);
        return NULL;
    }

    if (SSL_set_fd(ssl, sockfd) != 1) {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(config);
        return NULL;
    }

    // TLS handshake
    SSL_set_connect_state(ssl); // set as client
    int result = SSL_do_handshake(ssl);
    if (result != 1) {
        result = SSL_get_error(ssl, result);
        ERR_print_errors_fp(stderr);
    }


    SSL_CTX_free(config);

    return ssl;
}


int tls_send(SSL *ssl, char *message) {
    int result = SSL_write(ssl, message, strlen(message));

    if (result <= 0) return result;
    return 0;
}

int GoogleAuthentication(SSL *ssl, char *account, char *accessToken) {

    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        return 1;
    } else {
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.google.com/o/oauth2/auth?client_id=93409216094-boh7jt4f70ll96t5gtfboujembvoevjl.apps.googleusercontent.com&redirect_uri=http://localhos:8080/t&response_type=code&scope=https://mail.google.com/");
        result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            fprintf(stderr, "Google authentication failed\n");
            return 1;
        }

        curl_easy_cleanup(curl);
    }









    char credential[2048];
    sprintf(credential, "user=%s^Aauth=Bearer %s^A^A", account, accessToken);

    char encodedCredential[2048];
    EVP_EncodeBlock((unsigned char*)encodedCredential, (unsigned char*)credential, strlen(credential));

    tls_send(ssl, encodedCredential);

    return 0;
}
