typedef struct TextBufferData {
    GtkTextBuffer *textBuffer;
    GtkWidget     *statusbar;
    int           *serverSocket;
    char          lines[LINES_LIMIT][LINE_MAX_LENGTH];
    int           currentCursorLine;
//    gulong        onChangeSignalId;
} TextBufferData;
