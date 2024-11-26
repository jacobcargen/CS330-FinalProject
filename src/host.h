

// host.h

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "game.h"

#ifndef HOST_H
#define HOST_H


struct Player {
    int socket;
    bool isReady;
} typedef Player;


class Host {

public:
    // Constructor
    Host();
    // Public methods
    void start();
    void sendMessageToClient(Player* client, const std::string& dataStr, bool isClear);
    void promptClient(Player * client, const std::string &dataStr);
    std::string getReponseFromClientPrompt();

private:
    // Private
    int client_sockets[8] = {0};
};

#endif // HOST_H
