
enum messageType
{
    // Client
    READY = 1,
    CLOSE_CONNECTION = 255,

    // Host
    DRAW = 2,
    CLEAR = 3,
};

struct message {
    int data;
} typedef message;
