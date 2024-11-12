// client.cpp
#include <iostream>
#include "client.h"
#include "ui.h"
#include "message_helper.h"
#include "game.h"

// Constructor
Client::Client()
{
    test();
}

// Public method
void Client::test()
{
    std::cout << "Client Test" << std::endl;
    UI ui;
}

// Add anything client specific, 
