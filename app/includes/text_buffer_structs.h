typedef struct TextBufferData {
    GtkTextBuffer *textBuffer;
    GtkWidget     *statusbar;
    int           *serverSocket;
    int           currentCursorLine;
} TextBufferData;
