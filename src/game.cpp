// game.cpp
#include <iostream>
#include "game.h"

// Store players here
player * players;


// Constructor
Game::Game() {
    test();

    while (1)
    {
        gameLoop();
    }
    
}

// Public method
void Game::test() 
{
    std::cout << "Game Test" << std::endl;
}

void Game::waitForReady()
{
    // wait for all players to ready && players 1+
}

void Game::gameLoop()
{
    // wait for players to be ready, ignore if all already ready
    waitForReady();
    /////////////
    // DO STUFF HERE
    /////////////
}

void Game::updateClients()
{
    // update all the clients

    // For each client, send client new data (it is up to client to handle that data)
}


