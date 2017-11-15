#include <gtk/gtk.h>

void update_statusbar(GtkTextBuffer *buffer, GtkStatusbar *statusbar) {
    gchar       *msg;
    gint        row, col;
    GtkTextIter iter;

    gtk_statusbar_pop(statusbar, 0);

    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));

    row = gtk_text_iter_get_line(&iter);
    col = gtk_text_iter_get_line_offset(&iter);

    msg = g_strdup_printf("Col: %d Ln: %d", col + 1, row + 1);

    gtk_statusbar_push(statusbar, 0, msg);

    g_free(msg);
}

void mark_set_callback(GtkTextBuffer *buffer, const GtkTextIter *new_location, GtkTextMark *mark, gpointer data) {

    update_statusbar(buffer, GTK_STATUSBAR(data));
}

GtkWidget *prepareWindow() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 300);
    gtk_window_set_title(GTK_WINDOW(window), "Lines & columns");

    return window;
}

GtkWidget *prepareVerticalBox(GtkWidget *window) {
    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    return vbox;
}

GtkWidget *prepareToolbar() {
    GtkWidget *toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

    return toolbar;
}

GtkToolItem *prepareExitButton(GtkWidget *toolbar) {
    GtkToolItem *exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

    return exit;
}

GtkWidget *prepareTextView(GtkWidget *vbox) {
    GtkWidget *textView = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(vbox), textView, TRUE, TRUE, 0);
    gtk_widget_grab_focus(textView);

    return textView;
}

GtkWidget *prepareStatusBar(GtkWidget *vbox) {
    GtkWidget *statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);

    return statusbar;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget   *window  = prepareWindow();
    GtkWidget   *vbox    = prepareVerticalBox(window);
    GtkWidget   *toolbar = prepareToolbar();
    GtkToolItem *exit    = prepareExitButton(toolbar);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

    GtkWidget     *textView  = prepareTextView(vbox);
    GtkTextBuffer *buffer    = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    GtkWidget     *statusbar = prepareStatusBar(vbox);

    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buffer, "changed", G_CALLBACK(update_statusbar), statusbar);
    g_signal_connect_object(buffer, "mark_set", G_CALLBACK(mark_set_callback), statusbar, 0);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    update_statusbar(buffer, GTK_STATUSBAR(statusbar));

    gtk_main();

    return 0;
}