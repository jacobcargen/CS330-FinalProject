

// host.h

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "game.h"

#ifndef HOST_H
#define HOST_H


constexpr int PORT = 8080;
constexpr int MAX_CLIENTS = 8;
const std::string WELCOME_MSG = "Welcome!\n";



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
    void promptComplete();
    std::string getReponseFromClientPrompt();
    void reprompt(Player * client);

private:
    // Private

    int client_sockets[8] = {0};

    Player clients[MAX_CLIENTS] = {};

    Player * promptedClient = nullptr;
    std::string clientPromptResponse = "";
    std::string lastPrompt = "";
};

#endif // HOST_H
