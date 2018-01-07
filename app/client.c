#include <gtk/gtk.h>
#include "includes/prepares.h"
#include "includes/bindings.h"

int main(int argc, char *argv[]) {
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


    struct BindingData data;
    data.statusbar = statusbar;
    int serverSocket = connectToServer(SERVER_NAME, SERVER_PORT);
    data.serverSocket = &serverSocket;

    bindEventListeners(window, exit, buffer, &data);

    gtk_main();

    return 0;
}