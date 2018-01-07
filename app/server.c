//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
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

#define TRUE   1
#define FALSE  0
#define HOST "127.0.0.1"
#define PORT 8888
#define MAX_CLIENTS 30

struct CollaborativeEditorServer {
    struct sockaddr_in *address;

    int *client_sockets;
    int num_of_client_sockets;

    int server_socket;

    fd_set *read_fds;
};

void initialize_client_sockets(int* client_sockets, int size) {
    for (int i = 0; i < size; i++) {
        // Assigning zero to all sockets, so we won't have any strange values there
        client_sockets[i] = 0;
    }
}

int create_server_socket() {
    int server_socket = socket(AF_INET, SOCK_STREAM , 0);
    if (server_socket == 0) {
        perror("Error has occcured while creating server socket");
        exit(EXIT_FAILURE);
    }

    //Set master socket to allow multiple connections
    int multiple_conns = TRUE;
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&multiple_conns, sizeof(multiple_conns)) < 0 )
    {
        perror("Setsockopt has failed");
        exit(EXIT_FAILURE);
    }

    printf("Created server socket: %i\n", server_socket);
    return server_socket;
}

void initialize_address_structure(struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = inet_addr(HOST);
    address->sin_port = htons(PORT);
}

void bind_server_socket_to_port(int server_socket, struct sockaddr_in address) {
    //bind the socket to localhost port 8888
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d... \n", PORT);
}

void listen_on_socket(int master_socket) {
    if (listen(master_socket, MAX_CLIENTS) < 0) {
          perror("Error while listening");
          exit(EXIT_FAILURE);
    }
}

int reset_fd_set(struct CollaborativeEditorServer *server) {
    //Clear the socket set. On each event loop iteration the fd_set has to
    // be destroyed and re-created
    FD_ZERO(server->read_fds);

    //Add server socket to fd_set
    FD_SET(server->server_socket, server->read_fds);
    //Highest file descriptor number, needed for the select function
    int highest_file_descriptor = server->server_socket;

    //Add child sockets to set and determine highest file descriptor
    for (int i = 0; i < server->num_of_client_sockets; i++)
    {
        int socket_descriptor = server->client_sockets[i];
        if (socket_descriptor == 0) {
            // Means that description has not been allocated yet
            // and doesn't need to be added to the fdset we are listening on
            continue;
        }

        FD_SET(socket_descriptor, server->read_fds);
        if(socket_descriptor > highest_file_descriptor) {
            highest_file_descriptor = socket_descriptor;
        }
    }

    return highest_file_descriptor;
}

void add_new_socket_to_empty_client_slot(
    struct CollaborativeEditorServer *server,
    int new_socket
) {
    for (int i = 0; i < server->num_of_client_sockets; i++) {
        if(server->client_sockets[i] != 0) {
            continue;
        }

        server->client_sockets[i] = new_socket;
        printf("Adding socket: %d to list of sockets at index: %d\n" , new_socket, i);
        break;
    }
}

void handle_server_socket_activity(struct CollaborativeEditorServer *server) {
    //If something happened on the server socket, its an incoming connection
    if (!FD_ISSET(server->server_socket, server->read_fds)) {
        return;
    }

    int new_socket, addrlen;
    if ((new_socket = accept(
      server->server_socket, (struct sockaddr *) server->address,
      (socklen_t*)&addrlen)) < 0
    ) {
        perror("Error while accepting client connection");
        return;
    }

    add_new_socket_to_empty_client_slot(server, new_socket);

    // TODO: Send existing editor text
    char* message = "My message";
    send(new_socket , message , strlen(message) , 0);
}

void event_loop(struct CollaborativeEditorServer *server) {
    // 1K Buffer for incoming data
    char buffer[1025];

    while(TRUE) {
        int highest_file_descriptor = reset_fd_set(server);

        int activity = select(
            highest_file_descriptor + 1 , server->read_fds , NULL , NULL , NULL);

        if (activity < 0) {
            printf("Select error");
        }

        handle_server_socket_activity(server);
    }
}

int main(int argc , char *argv[])
{
    struct sockaddr_in address;
    initialize_address_structure(&address);

    int client_sockets[MAX_CLIENTS];
    initialize_client_sockets(client_sockets, MAX_CLIENTS);

    struct CollaborativeEditorServer server;
    server.server_socket = create_server_socket();
    server.address = &address;
    server.client_sockets = client_sockets;
    server.num_of_client_sockets = MAX_CLIENTS;

    bind_server_socket_to_port(server.server_socket, address);
    listen_on_socket(server.server_socket);

    event_loop(&server);

    //
    //     //else its some IO operation on some other socket
    //     for (i = 0; i < max_clients; i++)
    //     {
    //         sd = client_socket[i];
    //
    //         if (FD_ISSET( sd , &readfds))
    //         {
    //             //Check if it was for closing , and also read the
    //             //incoming message
    //             if ((valread = read( sd , buffer, 1024)) == 0)
    //             {
    //                 //Somebody disconnected , get his details and print
    //                 getpeername(sd , (struct sockaddr*)&address , \
    //                     (socklen_t*)&addrlen);
    //
    //
    //
    //
    //                 // printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
    //
    //                 //Close the socket and mark as 0 in list for reuse
    //                 close( sd );
    //                 client_socket[i] = 0;
    //             }
    //
    //             //Echo back the message that came in
    //             else
    //             {
    //                 //set the string terminating NULL byte on the end
    //                 //of the data read
    //                 buffer[valread] = '\0';
    //
    //                 char msg[1025];
    //                 snprintf(msg, sizeof msg, "%s%d%s%s", "Client #", i, ": ", buffer);
    //
    //                 for (int j = 0; j < max_clients; j++) {
    //                     if (client_socket[j] > 0 && j != i) {
    //
    //                         send(client_socket[j] , msg , strlen(msg) , 0 );
    //                     }
    //                 }
    //                 // send(sd1 , buffer , strlen(buffer) , 0 );
    //
    //                 printf("[app-log] %s\n", msg);
    //             }
    //
    //             // printf("Client #%d sent something\n", i);
    //         }
    //     }
    // }

    return 0;
}
