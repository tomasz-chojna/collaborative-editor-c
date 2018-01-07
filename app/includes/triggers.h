#include <gtk/gtk.h>
#include "client.h"

gint *getCursorCords(GtkTextBuffer *buffer, GtkTextIter *iter) {
    static gint cords[2];

    gtk_text_buffer_get_iter_at_mark(buffer, iter, gtk_text_buffer_get_insert(buffer));

    cords[0] = gtk_text_iter_get_line(iter);
    cords[1] = gtk_text_iter_get_line_offset(iter);

    return cords;
}

gchar *getCursorStatus(GtkTextBuffer *buffer) {
    GtkTextIter iter;
    gint        *cords = getCursorCords(buffer, &iter);

    gint row = cords[0];
    gint col = cords[1];

    return g_strdup_printf("Col: %d Ln: %d", col + 1, row + 1);
}

void update_statusbar(GtkTextBuffer *buffer, GtkStatusbar *statusbar) {
    gtk_statusbar_pop(statusbar, 0);
    gchar *msg = getCursorStatus(buffer);
    gtk_statusbar_push(statusbar, 0, msg);

    g_free(msg);
}

//void sendMessageToServer(gchar *msg) {
//    socketClient(SERVER_NAME, SERVER_PORT, msg);
//}

void getBoundsOfCurrentLine(const GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end) {
    GtkTextIter iter;
    gint        lineNumber;


    // mark the iter at the current position in the buffer
    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
    lineNumber = gtk_text_iter_get_line(&iter);

    // set start iterator to the very beginning of the line
    gtk_text_buffer_get_iter_at_line(buffer, start, lineNumber);

    // copy value of start to end
    (*end) = (*start);

    // and move it to the end of line
    gtk_text_iter_forward_to_line_end(end);
}

void sendTypedCharacterToServer(GtkTextBuffer *buffer, const int *serverSocket) {
    gchar *msg;
    GtkTextIter start, end;
    getBoundsOfCurrentLine(buffer, &start, &end);

    // retrieve message between the start and end
    msg = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);



    // todo: send data in format containing line number, modification type and text.
    // todo: create "modification type" resolver based on incoming text



    sendDataToServer(msg, *serverSocket);

    g_free(msg);
}

void mark_set_callback(GtkTextBuffer *buffer, const GtkTextIter *new_location, GtkTextMark *mark, gpointer data) {

    update_statusbar(buffer, GTK_STATUSBAR(data));
}