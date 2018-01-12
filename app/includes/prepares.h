GtkWidget *prepareWindow(char* title) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 300);
    gtk_window_set_title(GTK_WINDOW(window), title);

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
