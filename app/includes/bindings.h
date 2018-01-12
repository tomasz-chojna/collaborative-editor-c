#include <gtk/gtk.h>
#include "text_buffer_handler.h"

typedef struct BindingData {
    GtkWidget *statusbar;
    int       *serverSocket;
} BindingData;

struct TextViewWithSocket {
    GtkTextBuffer* textBuffer;
    int clientSocket;
    char lines[LINES_LIMIT][LINE_MAX_LENGTH];
};

void killClient() {
    clientIsWorking = FALSE;
}

void bindOnExit(const GtkWidget *window, const GtkToolItem *exit, TextBufferData *data) {
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(killClient), NULL);

    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(killClient), NULL);
}

void bindOnChangeSendModifiedLinesToServer(TextBufferData *data) {
    data->onChangeSignalId = g_signal_connect(data->textBuffer, "changed", G_CALLBACK(sendModifiedLinesToServer), data);
}

void unbindOnChangeSendModifiedLinesToServer(TextBufferData *data) {
    g_signal_handler_disconnect(data->textBuffer, data->onChangeSignalId);
}

void bindOnBufferChanged(TextBufferData *data) {

    g_signal_connect(data->textBuffer, "mark_set", G_CALLBACK(setCurrentCursorLine), data);
    bindOnChangeSendModifiedLinesToServer(data);
//    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), data->statusbar, 0);
}

void initStatusBar(TextBufferData *data) {
    updateStatusbar(data->textBuffer, GTK_STATUSBAR(data->statusbar));
}

void bindEventListeners(
    GtkWidget *window,
    GtkToolItem *exit,
    TextBufferData *data
) {

    bindOnExit(window, exit, data);
    bindOnBufferChanged(data);

    initStatusBar(data);
}