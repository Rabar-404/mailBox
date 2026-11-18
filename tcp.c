#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#define ERROR -1
#define RECEIVE_MODE 0

/**
 * Do a TCP handshake
 * @return socket file descriptor if successful, -1 on error
 */
int tcp_handshake(char *hostName, char *portNumber) {
    struct addrinfo hint = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP};

    struct addrinfo *respond = NULL;

    int returnNum = getaddrinfo(hostName, portNumber, &hint, &respond);
    if (returnNum)
    {
        fprintf(stderr, "Error: %s\n", gai_strerror(returnNum));
        return ERROR;
    }

    int sock_fd = socket(respond->ai_family, respond->ai_socktype, respond->ai_protocol);
    if (sock_fd == -1)
    {
        fprintf(stderr, "Error: socket create failed\n");
        return ERROR;
    }

    freeaddrinfo(respond);

    returnNum = connect(sock_fd, respond->ai_addr, respond->ai_addrlen);
    if (returnNum != 0)
    {

        perror("connect");
        return ERROR;
    }

    return sock_fd;
}

