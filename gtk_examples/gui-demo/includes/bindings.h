#include <gtk/gtk.h>
#include "triggers.h"

void onExit(const GtkWidget *window, const GtkToolItem *exit) {
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void onBufferChanged(GtkTextBuffer *buffer, GtkWidget *statusbar) {
    g_signal_connect(buffer, "changed", G_CALLBACK(update_statusbar), statusbar);
    g_signal_connect(buffer, "changed", G_CALLBACK(sendCursorStatusToServer), statusbar);
    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), statusbar, 0);
}

void initStatusBar(GtkTextBuffer *buffer, GtkWidget *statusbar) {
    update_statusbar(buffer, GTK_STATUSBAR(statusbar));
}

void bindEventListeners(
        const GtkWidget *window,
        const GtkToolItem *exit,
        GtkTextBuffer *buffer,
        GtkWidget *statusbar
) {
    onExit(window, exit);
    onBufferChanged(buffer, statusbar);

    initStatusBar(buffer, statusbar);
}