#include <gtk/gtk.h>
#include "triggers.h"

typedef struct BindingData {
    GtkWidget *statusbar;
    int       *serverSocket;
} BindingData;

void onExit(const GtkWidget *window, const GtkToolItem *exit, BindingData *data) {
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void onBufferChanged(GtkTextBuffer *buffer, BindingData *data) {
    g_signal_connect(buffer, "changed", G_CALLBACK(update_statusbar), data->statusbar);
    g_signal_connect(buffer, "changed", G_CALLBACK(sendTypedCharacterToServer), data->serverSocket);
    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), data->statusbar, 0);
}

void initStatusBar(GtkTextBuffer *buffer, BindingData *data) {
    update_statusbar(buffer, GTK_STATUSBAR(data->statusbar));
}

void bindEventListeners(
        GtkWidget *window,
        GtkToolItem *exit,
        GtkTextBuffer *buffer,
        GtkWidget *statusbar,
        int *serverSocket
) {
    BindingData data;
    data.statusbar    = statusbar;
    data.serverSocket = serverSocket;

    onExit(window, exit, &data);
    onBufferChanged(buffer, &data);

    initStatusBar(buffer, &data);
}