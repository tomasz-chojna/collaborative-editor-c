#include <pthread.h>
#include <gtk/gtk.h>
#include "includes/common.h"
#include "includes/prepares.h"
#include "includes/bindings.h"


int main(int argc, char *argv[]) {
    int serverSocket = connectToServer(HOST, PORT);

    gtk_init(&argc, &argv);

    GtkWidget   *window  = prepareWindow("Collaborative editor");
    GtkWidget   *vbox    = prepareVerticalBox(window);
    GtkWidget   *toolbar = prepareToolbar();
    GtkToolItem *exit    = prepareExitButton(toolbar);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

    GtkWidget     *textView  = prepareTextView(vbox);
    GtkTextBuffer *buffer    = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    GtkWidget     *statusbar = prepareStatusBar(vbox);

    gtk_widget_show_all(window);


    TextBufferData *data = malloc(sizeof(TextBufferData));
    data->statusbar    = statusbar;
    data->textBuffer   = buffer;
    data->serverSocket = &serverSocket;
    bindEventListeners(window, exit, data);

//    struct TextViewWithSocket *textViewWithSocket = malloc(sizeof(struct TextViewWithSocket));
//    textViewWithSocket->textBuffer   = buffer;
//    textViewWithSocket->clientSocket = serverSocket;

    eventLoops(data);

    return 0;
}

void *gtkListener() {
    gtk_main();
}

void reloadText(GtkTextBuffer *textBuffer, message_t *receivedMessage, char lines[LINES_LIMIT][LINE_MAX_LENGTH]) {
//    size_t size     = LINES_LIMIT * LINE_MAX_LENGTH;
    char *message = messageToString(receivedMessage);

    for (int i = 0; i < LINES_LIMIT; i++) {
        strcat(message, lines[i]);
        size_t len = strlen(message);

        message[len] = '\n';
    }
//
//    gtk_text_buffer_set_text(textBuffer, message, strlen(message));
}

void resolveIncomingMessage(message_t *message, TextBufferData *bufferData) {
    GtkTextIter start, end;

    unbindOnChangeSendModifiedLinesToServer(bufferData);

    switch (message->type) {
        case SERVER_FINISHED_SENDING_DATA:
            bindOnChangeSendModifiedLinesToServer(bufferData);
            return;
        case LINE_ADDED:
            gtk_text_buffer_get_iter_at_line(bufferData->textBuffer, &start, message->row);
            gtk_text_buffer_insert(bufferData->textBuffer, &start, "\n", 1);

            break;
        case LINE_REMOVED:
            gtk_text_buffer_get_iter_at_line(bufferData->textBuffer, &start, message->row);
            gtk_text_buffer_backspace(bufferData->textBuffer, &start, FALSE, TRUE);

            break;
        case LINE_MODIFIED:
        default:
            gtk_text_buffer_get_iter_at_line(bufferData->textBuffer, &start, message->row);

            getBoundsOfLine(bufferData->textBuffer, message->row, &start, &end);
            gtk_text_buffer_delete(bufferData->textBuffer, &start, &end);

            gtk_text_buffer_insert(bufferData->textBuffer, &start, message->text, strlen(message->text));

            break;
    }

//        strcpy(textViewWithSocket->lines[message.row], message.text);
//        reloadText(textViewWithSocket->textBuffer, &message, textViewWithSocket->lines);

}

void *incomingMessageListener(void *threadContext) {
    TextBufferData *textViewWithSocket = (TextBufferData *) threadContext;

    for (int i = 0; i < LINES_LIMIT; i++) {
        strcpy(textViewWithSocket->lines[i], "");
    }

    while (TRUE) {
        size_t messageSize   = sizeof(message_t);
        char   *socketBuffer = malloc(messageSize);

        if (recv(*textViewWithSocket->serverSocket, socketBuffer, messageSize, NULL) != -1) {
            message_t receivedMessage;
            memcpy(&receivedMessage, socketBuffer, messageSize);

            resolveIncomingMessage(&receivedMessage, textViewWithSocket);
        }
    }

    free(textViewWithSocket);
}

void eventLoops(TextBufferData *textViewWithSocket) {
    pthread_t thread[2];

    //starting the thread
    pthread_create(&thread[0], NULL, gtkListener, NULL);
    for (int i =0; i <= 100000000; i++);
    pthread_create(&thread[1], NULL, incomingMessageListener, textViewWithSocket);

    //waiting for completion
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
}

char *messageToString(message_t *message) {
    size_t size    = sizeof(message_t);
    char   *buffer = malloc(size);
    memset(buffer, 0x00, size);
    memcpy(buffer, message, size);

    return buffer;
}

void sendMessageToServer(message_t *message, int serverSocket) {
    if ((send(serverSocket, messageToString(message), sizeof(message_t), 0)) < 0) {
        perror("send() failed");
        exit(EXIT_FAILURE);
    }
}


/**
 * Closes socket descriptor
 */
void disconnectFromServer(int serverSocket) {
    if (serverSocket != -1) close(serverSocket);
}

/**
 * Pass in 1 parameter which is either the
 * address or host name of the app, or
 * set the app name in the #define
 * SERVER_NAME.
 */
int connectToServer(char *server_name, int server_port) {
    // Variable and structure definitions.
    int                serverSocket = -1;
    char               host[128];
    struct sockaddr_in serverAddress;
    struct hostent     *hostp;

    /*
     * The socket() function returns a socket descriptor representing
     * an endpoint.  The statement also identifies that the INET
     * (Internet Protocol) address family with the TCP transport
     * (SOCK_STREAM) will be used for this socket.
     */
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    /*
     * If an argument was passed in, use this as the app, otherwise
     * use the #define that is located at the top of this program.
     */
    strcpy(host, server_name);

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_port        = htons(server_port);
    serverAddress.sin_addr.s_addr = inet_addr(host);
    if (serverAddress.sin_addr.s_addr == (unsigned long) INADDR_NONE) {

        /*
         * The app string that was passed into the inet_addr()
         * function was not a dotted decimal IP address.  It must
         * therefore be the hostname of the app.  Use the
         * gethostbyname() function to retrieve the IP address of the
         * app.
         */
        hostp = gethostbyname(host);
        if (hostp == (struct hostent *) NULL) {
            printf("Host not found --> ");
            printf("h_errno = %d\n", h_errno);
            exit(EXIT_FAILURE);
        }

        memcpy(&serverAddress.sin_addr, hostp->h_addr, sizeof(serverAddress.sin_addr));
    }

    /*
     * Use the connect() function to establish a connection to the app.
     */
    if (connect(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("connect() failed");
        exit(EXIT_FAILURE);
    }

    return serverSocket;
}