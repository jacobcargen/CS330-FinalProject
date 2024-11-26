#include <iostream>
#include "client.h"
#include "ui.h"
#include "message_helper.h"

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Constructor
Client::Client()
{
    Start();
}

// Private functions
void Client::Start()
{
    UI ui;

    // Prompt for server IP address
    std::string ip = ui.promptLine("Server IP:");
    // Attempt to join the server
    if (ip.compare("local"))
       std::string ip = "127.0.0.1";
    Join(ip);
}

void Client::Join(std::string ip)
{
    UI ui;
    const int PORT = 8080; // Server port
    int sock = 0;          // Client socket descriptor
    sockaddr_in serv_addr; // Server address struct

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << "\n";
        return;
    }

    serv_addr.sin_family = AF_INET;      // IPv4
    serv_addr.sin_port = htons(PORT);   // Set port

    // Convert IP address from string to binary
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) 
    {
        std::cerr << "Invalid address or address not supported\n";
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        std::cerr << "Connection to server failed: " << strerror(errno) << "\n";
        return;
    }

    std::cout << "Connected to server!\n";

    // Main Game Loop
    while (true) 
    {
        // Receive display update from server
        char buffer[MESSAGE_SIZE] = {0};
        int valread = read(sock, buffer, sizeof(buffer) - 1);
        
        if (valread <= 0) 
        {
            if (valread == 0)
            {
                std::cout << "Server disconnected.\n";
                break; // Try again
            }
            else
            {
                std::cout << "Read failed.\n";
                continue;
            }
            
        }

        buffer[valread] = '\0'; // Null-terminate the buffer
        std::string serverMessage(buffer);

        if (serverMessage.find(CLEAR_MSG) != std::string::npos)
        {
            ui.clearScreen();
        }
        else if (serverMessage.find(PROMPT_MSG) != std::string::npos)
        {
            
            std::string input = ui.promptLine(serverMessage);
            
            send(sock, input.c_str(), input.size(), 0);
        }
        else //Print anything else
        {
            std::cout << serverMessage << std::endl;
        }
    }

    // Close the socket when done
    close(sock);

    
}
