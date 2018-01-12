#include <gtk/gtk.h>
#include "text_buffer_handler.h"

void killClient() {
    clientIsWorking = FALSE;
}

void onExit(const GtkWidget *window, const GtkToolItem *exit, TextBufferData *data) {
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(killClient), NULL);

    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(killClient), NULL);
}

void onBufferChanged(GtkTextBuffer *buffer, TextBufferData *data) {

    g_signal_connect(buffer, "mark_set", G_CALLBACK(setCurrentCursorLine), data);
    g_signal_connect(buffer, "changed", G_CALLBACK(sendModifiedLinesToServer), data);
//    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), data->statusbar, 0);
}

void initStatusBar(GtkTextBuffer *buffer, TextBufferData *data) {
    updateStatusbar(buffer, GTK_STATUSBAR(data->statusbar));
}

void bindEventListeners(
    GtkWidget *window,
    GtkToolItem *exit,
    GtkTextBuffer *buffer,
    TextBufferData *data
) {

    onExit(window, exit, data);
    onBufferChanged(buffer, data);

    initStatusBar(buffer, data);
}