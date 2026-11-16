#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#define RECEIVE_MODE 0
#define SEND_MODE 0

int receivemsg(char *buffer, ssize_t size, int sockfd, int receiveMode);


int smtp_connect(const char *hostName, const char *portNumber) {
    //test params
    hostName = "smtp.gmail.com";
    portNumber = "587";



    struct addrinfo hint = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    struct addrinfo *respond = NULL;

    int returnNum =  getaddrinfo(hostName, portNumber, &hint, &respond);
    if (returnNum) {
        fprintf(stderr, "Error: %s\n", gai_strerror(returnNum));
        return 1;
    }

    int sock_fd = socket(respond->ai_family, respond->ai_socktype, respond->ai_protocol);
    if (sock_fd == -1)
    {
        fprintf(stderr, "Error: socket create failed\n");
        return 1;
    }

    freeaddrinfo(respond);





    // TCP handshake
    returnNum = connect(sock_fd, respond->ai_addr, respond->ai_addrlen);
    if (returnNum != 0) {

        perror("connect");
        return 1;
    }
    char buffer[1024];
    int result = receivemsg(buffer, 1024, sock_fd, RECEIVE_MODE);
    

    // SMTP communication
    char *command = "EHLO localhost\t\n";
    send(sock_fd, command, strlen(command), SEND_MODE);
    result = receivemsg(buffer, 1024, sock_fd, RECEIVE_MODE);

    command = "STARTTLS\t\n";
    send(sock_fd, command, strlen(command), SEND_MODE);
    receivemsg(buffer, 1024, sock_fd, RECEIVE_MODE);


    // TLS handshake
    

    close(sock_fd);
}




int receivemsg(char *buffer, ssize_t size, int sockfd, int receiveMode) {
    ssize_t bytsReceived = recv(sockfd, buffer, size - 1, receiveMode);

    if (bytsReceived == -1) {
        perror("Failed to receive data!\n");
        return 1;
    } else if (bytsReceived == 0) {
        printf("Server disconnected\n");
    }
    buffer[bytsReceived] = '\0';

    printf("%s", buffer);

    return 0;
}