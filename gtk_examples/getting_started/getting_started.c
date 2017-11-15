#include <gtk/gtk.h>

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */
    return FALSE;
}

void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    /*
    This sets up a few things for us such as the default visual and color map
    and then proceeds to call gdk_init(gint *argc, gchar ***argv). This function
    initializes the library for use, sets up default signal handlers, and
    checks the arguments passed to your application on the command line
    */
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(window, "Collaborative editor");

    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}
