#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>

#include <sys/socket.h>


#define RECEIVE_MODE 0
#define SEND_MODE 0


/**
 * Start a SMTP communication via given sock fd
 * @param sock_fd file descriptor opened by TCP handshake
 * @return 0 if successful, other number on error
 */
int smtp_send(int sock_fd, char *message) {

    // SMTP communication
    send(sock_fd, message, strlen(message), SEND_MODE);
    return 0;
}
