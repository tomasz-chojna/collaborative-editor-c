#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "includes/structs.h"

int main() {
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_port        = htons(8888);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(socket_id, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    message_t m;
    m.type = LINE_MODIFIED;
    m.row = 2;
    strcpy(m.text, "To jest inna pierwsza linijka");

    int size = sizeof(message_t);
    char* buffer = malloc(size);
    memset(buffer, 0x00, size);
    memcpy(buffer, &m, size);

    send(socket_id , buffer , size, 0);
    close(socket_id);
    free(buffer);

    return 0;
}
