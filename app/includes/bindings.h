#include <gtk/gtk.h>
#include "triggers.h"

struct data {
    GtkWidget * statusbar;
    int * serverSocket;
};

void onExit(const GtkWidget *window, const GtkToolItem *exit, struct data *data) {
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(disconnectFromServer), data->serverSocket);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void onBufferChanged(GtkTextBuffer *buffer, struct data *data) {
    g_signal_connect(buffer, "changed", G_CALLBACK(update_statusbar), data->statusbar);
    g_signal_connect(buffer, "changed", G_CALLBACK(sendTypedCharacterToServer), data->serverSocket);
    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), data->statusbar, 0);
}

void initStatusBar(GtkTextBuffer *buffer, GtkWidget *statusbar) {
    update_statusbar(buffer, GTK_STATUSBAR(statusbar));
}

void bindEventListeners(
        const GtkWidget *window,
        const GtkToolItem *exit,
        GtkTextBuffer *buffer,
        struct data * data
) {
    onExit(window, exit, data);
    onBufferChanged(buffer, data);

    initStatusBar(buffer, data->statusbar);
}