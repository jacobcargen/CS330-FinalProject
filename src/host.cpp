#include <iostream>
#include "host.h"
#include "message_helper.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "game.h"
#include <vector>

constexpr int PORT = 8080;
constexpr int MAX_CLIENTS = 8;
const std::string WELCOME_MSG = "Welcome!\n";

Player clients[MAX_CLIENTS] = {};

Player * promptedClient = nullptr;
std::string clientPromptResponse = "";

// Constructor: Starts the host server when an object of Host is created
Host::Host() 
{
    start(); 
}

// Main server function
void Host::start() 
{
    
    Game game;
    game.init(this);

    int server_fd;                     // File descriptor for the server socket
    sockaddr_in address{};             // Struct for storing address info
    int addrlen = sizeof(address);     // Length of the address struct

    // Socket setup
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (server_fd < 0) 
    {
        perror("Socket creation failed");
        return;
    }

    // Configure the address for the socket
    address.sin_family = AF_INET;         // Use IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(PORT);      // Set the port to listen on
    int opt = 1;
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the configured address and port, and start listening
    if (bind(server_fd, (sockaddr*)&address, addrlen) < 0 || listen(server_fd, MAX_CLIENTS) < 0) 
    {
        perror("Bind/Listen failed");
        close(server_fd);
        return;
    }
    std::cout << "Listening on port " << PORT << std::endl;

    // Main loop to handle incoming connections and messages
    fd_set readfds;
    while (true) 
    {

        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        int max_sd = server_fd;
        for (const Player& client : clients) 
        {
            if (client.socket > 0) FD_SET(client.socket, &readfds);
            max_sd = std::max(max_sd, client.socket);
        }

        
        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        if (activity <= 0)
            return;
        
        // Handle new connections
        if (!game.hasStarted() && FD_ISSET(server_fd, &readfds)) 
        {
            int new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
            if (new_socket < 0) 
            {
                perror("Accept failed");
                continue;
            }

            for (Player& client : clients) 
            {
                if (client.socket == 0) 
                {
                    client.socket = new_socket;
                    std::cout << "Client[" << client.socket << "] has joined the server.\n";
                    sendMessageToClient(&client, WELCOME_MSG, false);
                    sendMessageToClient(&client, PROMPT_MSG + "Type ('x') to ready-up: ", false);
                    /*
                    UI ui;
                    std::string str = ui.getTestHand() + "\n\n\n\n\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
                    std::cout << "length:" << str.length() << std::endl;
                    sendMessageToClient(&client, str);
                    */
                    break;
                }
            }
        }

        // Handle messages from existing clients
        for (Player& client : clients) 
        {
            if (client.socket > 0 && FD_ISSET(client.socket, &readfds)) 
            {
                char buffer[MESSAGE_SIZE] = {0};
                int valread = read(client.socket, buffer, sizeof(buffer));

                if (valread == 0) // Client disconnected
                {
                    std::cout << "Client[" << client.socket << "] disconnected.\n";
                    close(client.socket);
                    client = {}; // Reset the client object
                }
                else 
                {
                    buffer[valread] = '\0'; // Null-terminate the received data
                    std::string clientMsg(buffer);
                    std::cout << "Client[" << client.socket << "]: " << clientMsg << "\n";
                    if (promptedClient == &client)
                    {
                        clientPromptResponse = clientMsg;
                    }
                    // Handle "ready up" logic
                    if (!client.isReady && clientMsg == "x") 
                    {
                        // Add player to the game
                        game.AddPlayer("Player" + std::to_string(client.socket), &client);
                        client.isReady = true;

                        // Check if all clients are ready
                        bool allReady = true;
                        for (const Player& c : clients) 
                        {
                            if (c.socket > 0 && !c.isReady) 
                            {
                                allReady = false;
                                break;
                            }
                        }

                        if (allReady)
                        {
                            std::cout << "All players are ready! Starting game...\n";
                            game.setGameAsReady();
                        }
                    }


                }
            }
        }
        if (game.hasStarted())
        {
            game.gameTick();
        }
    }

    close(server_fd);
}

void Host::sendMessageToClient(Player* client, const std::string& dataStr, bool isClear) 
{
    // Make sure 

    if (!client || client->socket <= 0) 
    {
        std::cerr << "Invalid client or socket.\n";
        return;
    }

    // Step 1: Send the clear screen message if requested
    if (isClear) 
    {
        std::cout << "Server --> Client[" << client->socket << "]: \n" << CLEAR_MSG;

        std::string clearMsg = CLEAR_MSG;
        if (send(client->socket, clearMsg.c_str(), clearMsg.size(), 0) < 0) 
        {

            std::cerr << "Error sending clear message: " << strerror(errno) << "\n";
            return;
        }
    }

    // Step 2: Send the main message
    const std::string formattedMessage = isClear ? std::string("\u200B \n" + dataStr + "\n") : (dataStr);
                                                                
    std::cout << "Server --> Client[" << client->socket << "]: \n" << formattedMessage;
    if (send(client->socket, formattedMessage.c_str(), formattedMessage.size(), 0) < 0) 
    {
        std::cerr << "Error sending data message: " << strerror(errno) << "\n";
    }
}


void Host::promptClient(Player * client, const std::string &dataStr)
{
    promptedClient = client;
    clientPromptResponse = "";
    std::string data = PROMPT_MSG + dataStr;
    sendMessageToClient(client, data, false);
}
std::string Host::getReponseFromClientPrompt()
{
    if (clientPromptResponse == "")
        return "";
    std::string response = clientPromptResponse;
    clientPromptResponse = "";
    promptedClient = nullptr;
    return response;
}