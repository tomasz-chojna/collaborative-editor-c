#include <gtk/gtk.h>

// Remember you should not put ; at the end of #define line
// it wouldn't work
#define WINDOW_TITLE "Collaborative editor"
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 500


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

void initialize_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    gtk_widget_set_size_request(GTK_WIDGET(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);

    GtkWidget *ipAddressLabel = gtk_label_new("IP Address:");
    gtk_widget_show(ipAddressLabel);

    GtkWidget *ipAddressEntry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(ipAddressEntry), 13);
    gtk_entry_set_placeholder_text(GTK_ENTRY(ipAddressEntry), "Ip address");
    gtk_widget_show(ipAddressEntry);

    GtkWidget *portLabel = gtk_label_new("Port:");
    gtk_widget_show(portLabel);

    GtkWidget *portEntry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(portEntry), 13);
    gtk_entry_set_placeholder_text(GTK_ENTRY(portEntry), "Port");
    gtk_widget_show(portEntry);


    GtkWidget *button = gtk_button_new_with_label("Connect");
    gtk_widget_show(button);

    // 3 rows, 1 column
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), ipAddressLabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ipAddressEntry, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), portLabel, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), portEntry, 0, 3, 1, 1);

    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_show(grid);

    // A GtkVBox is a container that organizes child widgets into a single column.
    // second argument is spacing - number of pixels to place by default between
    // children
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    // Adds child to box , packed with reference to the start of box .
    // The child is packed after any other child packed with reference to the start of box .
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    gtk_widget_show(window);
}

int main(int argc, char *argv[]) {
    /*
    This sets up a few things for us such as the default visual and color map
    and then proceeds to call gdk_init(gint *argc, gchar ***argv). This function
    initializes the library for use, sets up default signal handlers, and
    checks the arguments passed to your application on the command line
    */
    gtk_init(&argc, &argv);

    initialize_window();

    gtk_main();

    return 0;
}
