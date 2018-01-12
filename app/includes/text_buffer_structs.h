#import <gtk/gtk.h>


typedef struct TextBufferData {
    GtkWidget *statusbar;
    int       *serverSocket;
    int       currentCursorLine;
} TextBufferData;
