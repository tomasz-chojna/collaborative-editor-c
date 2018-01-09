#include <gtk/gtk.h>
#include "client.h"

enum MessageType resolveMessageType(const GtkTextBuffer *buffer) {

    // TODO: real resolver of message type

    return LINE_MODIFIED;
}

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

void updateStatusbar(GtkTextBuffer *buffer, GtkStatusbar *statusbar) {
    gtk_statusbar_pop(statusbar, 0);
    gchar *msg = getCursorStatus(buffer);
    gtk_statusbar_push(statusbar, 0, msg);

    g_free(msg);
}

gint currentLineNumber(const GtkTextBuffer *buffer) {
    GtkTextIter iter;

    // mark the iter at the current position in the buffer
    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));

    return gtk_text_iter_get_line(&iter);
}

void getBoundsOfCurrentLine(const GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end) {
    gint lineNumber = currentLineNumber(buffer);
    // set start iterator to the very beginning of the line
    gtk_text_buffer_get_iter_at_line(buffer, start, lineNumber);
    // copy value of start to end
    (*end) = (*start);
    // and move it to the end of line
    gtk_text_iter_forward_to_line_end(end);
}

gchar *getCurrentLineText(const GtkTextBuffer *buffer) {
    GtkTextIter start, end;
    getBoundsOfCurrentLine(buffer, &start, &end);

    // retrieve message between the start and end
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

message_t *messageFromTextBuffer(const GtkTextBuffer *buffer) {
    message_t *messageToSend = malloc(sizeof(message_t));

    messageToSend->type = resolveMessageType(buffer);
    messageToSend->row  = currentLineNumber(buffer);
    strcpy(messageToSend->text, getCurrentLineText(buffer));

    return messageToSend;
}

void sendCurrentLineToServer(GtkTextBuffer *buffer, const int *serverSocket) {
    sendMessageToServer(
        messageFromTextBuffer(buffer),
        *serverSocket
    );
}

void mark_set_callback(GtkTextBuffer *buffer, const GtkTextIter *new_location, GtkTextMark *mark, gpointer data) {

    updateStatusbar(buffer, GTK_STATUSBAR(data));
}