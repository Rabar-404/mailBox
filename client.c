#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include "smtp.h"
#include "tcp.h"
#include "tls.h"


static int printReceivedMsg(int sockfd);
static int printTLSMsg(SSL *ssl);
int handshake(char *hostName, char *portNumber);

int main () {
    
    handshake("smtp.gmail.com", "587");
    return 0;
}

int handshake(char *hostName, char *portNumber) {
    int socket_fd = tcp_handshake(hostName, portNumber);
    printReceivedMsg(socket_fd);

    smtp_send(socket_fd, "EHLO localhost\t\n");
    printReceivedMsg(socket_fd);

    smtp_send(socket_fd, "STARTTLS\t\n");
    printReceivedMsg(socket_fd);
    
    // TLS handshake
    SSL *ssl = tls_handshake(socket_fd);
    if (ssl == NULL) {
        fprintf(stderr, "Error: errno in tls handshake\n");
        return 1;
    }

    tls_send(ssl, "EHLO localhost\t\n");
    printTLSMsg(ssl);

    GoogleAuthentication(ssl, "j212246541@gmail.com", "123");


    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(socket_fd);
    return 0;
}

static int printReceivedMsg(int sockfd) {
    char buffer[1024];
    const int MODE = 0;

    ssize_t size = recv(sockfd, buffer, 1024, MODE);

    if (size == -1) {
        perror("Failed to receive data!\n");
        return 1;
    } else if (size == 0) {
        printf("Server disconnected\n");
    }
    
    buffer[size] = '\0';

    printf("%s", buffer);

    return 0;
}

static int printTLSMsg(SSL *ssl) {
    char buffer[1024];

    ssize_t size = SSL_read(ssl, buffer, 1024);

    if (size == -1) {
        perror("Failed to receive data!\n");
        return 1;
    } else if (size == 0) {
        printf("Server disconnected\n");
    }
    
    buffer[size] = '\0';

    printf("%s", buffer);

    return 0;
}