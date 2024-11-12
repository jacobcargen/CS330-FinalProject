// host.cpp
#include <iostream>
#include "host.h"
#include "message_helper.h"
#include "game.h"

// Constructor
Host::Host()
{
    test();
}

// Public method
void Host::test()
{
    std::cout << "Host Test" << std::endl;

    Game game;
}

//void Host::giveCard()
// Start
    // create game
    // wait for players to join
    // handle card dealing
    // 