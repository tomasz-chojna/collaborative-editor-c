/**************************************************************************/
/* This sample program provides a code for a connection-oriented client.  */
/**************************************************************************/

/**************************************************************************/
/* Header files needed for this sample program                            */
/**************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include <netdb.h>

#define NETDB_MAX_HOST_NAME_LENGTH 128

/**************************************************************************/
/* Constants used by this program                                         */
/**************************************************************************/
#define SERVER_PORT     8888
#define SERVER_NAME     "localhost"
#define BUFFER_LENGTH    250
#define FALSE              0

void disconnectFromServer(int serverSocket);

void sendDataToServer(const gchar *message, int serverSocket);

void delay(int number_of_seconds) {
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}

/* Pass in 1 parameter which is either the */
/* address or host name of the app, or  */
/* set the app name in the #define      */
/* SERVER_NAME.                             */
int connectToServer(char *server_name, int server_port) {
    /***********************************************************************/
    /* Variable and structure definitions.                                 */
    /***********************************************************************/
    int                serverSocket = -1, rc, bytesReceived;
//    char               buffer[BUFFER_LENGTH];
    char               server_address[NETDB_MAX_HOST_NAME_LENGTH];
    struct sockaddr_in serveraddr;
    struct hostent     *hostp;

    /********************************************************************/
    /* The socket() function returns a socket descriptor representing   */
    /* an endpoint.  The statement also identifies that the INET        */
    /* (Internet Protocol) address family with the TCP transport        */
    /* (SOCK_STREAM) will be used for this socket.                      */
    /********************************************************************/

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        return -1;
    }

    /********************************************************************/
    /* If an argument was passed in, use this as the app, otherwise  */
    /* use the #define that is located at the top of this program.      */
    /********************************************************************/

    strcpy(server_address, server_name);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_port        = htons(server_port);
    serveraddr.sin_addr.s_addr = inet_addr(server_address);
    if (serveraddr.sin_addr.s_addr == (unsigned long) INADDR_NONE) {
        /*****************************************************************/
        /* The app string that was passed into the inet_addr()        */
        /* function was not a dotted decimal IP address.  It must        */
        /* therefore be the hostname of the app.  Use the             */
        /* gethostbyname() function to retrieve the IP address of the    */
        /* app.                                                       */
        /*****************************************************************/

        hostp = gethostbyname(server_address);
        if (hostp == (struct hostent *) NULL) {
            printf("Host not found --> ");
            printf("h_errno = %d\n", h_errno);
            return -1;
        }

        memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));
    }

    /********************************************************************/
    /* Use the connect() function to establish a connection to the      */
    /* app.                                                          */
    /********************************************************************/
    if (connect(serverSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        perror("connect() failed");
        return -1;
    }

    return serverSocket;
}

void sendDataToServer(const gchar *message, int serverSocket) {
    /********************************************************************/
    /* Send 250 bytes of a's to the app                              */
    /********************************************************************/
//        memset(buffer, 'a', sizeof(buffer));
    char buffer[BUFFER_LENGTH];
    strcpy(buffer, message);

    if ((send(serverSocket, buffer, sizeof(buffer), 0)) < 0) {
        perror("send() failed");
//        return -1;
    }
}

void disconnectFromServer(int serverSocket) {
    /***********************************************************************/
    /* Close down any open socket descriptors                              */
    /***********************************************************************/
    if (serverSocket != -1) close(serverSocket);
}