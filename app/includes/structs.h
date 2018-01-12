typedef struct Message {
    int type;
    int row;
    char text[120];
} message_t;

typedef enum MessageType {
    LINE_MODIFIED = 1,
    LINE_ADDED = 2,
    LINE_REMOVED = 3,
    SERVER_FINISHED_SENDING_DATA = 4
} message_type_t;
