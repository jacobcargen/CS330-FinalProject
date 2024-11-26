// game.cpp
#include <iostream>
#include "game.h"
#include "message_helper.h"
#include "host.h"
#include "game.h"

// Store players here
std::vector<gamePlayer> players;

gamePlayer * currentPlayerTurn;
int pTurn = 0;

bool isStarted = false;
Host * host;
bool waitingForPlayer = false;

// Constructor
Game::Game() 
{
    
}

void Game::init(Host * h)
{
    host = h;
    test();

}

// Public method
void Game::test() 
{
    std::cout << "Game Test" << std::endl;
}

bool Game::hasStarted()
{
    return isStarted;
}
void Game::setGameAsReady()
{
    isStarted = true;
    std::cout << "Game has been set as ready (lets just deal cards if a player wants one).\n";

    dealCardsTestGameExample();
}

void Game::dealCardsTestGameExample()
{
    std::cout << "dealCardsTestGameExample\n";
    // GAME SETUP
    UI ui;

    updateClients();

    // Clear hands
    for (gamePlayer p : players)
    {
        
        p.cards.clear();
    }

    std::string displayString = "TEMPLETE";
    // SPECIFIC GAME LOOP
    int pTurn = 0; // Start with the first player's turn
    while (true) // Replace `true` with your termination condition
    {
        // Get current player
        gamePlayer currentPlayer = players.at(pTurn);

        // Notify the current player
        host->sendMessageToClient(currentPlayer.client, "You're up!\n", false);

        // Add a test card to the current player's hand
        card testCard;
        testCard.face = ACE;
        testCard.suit = SPADES;
        currentPlayer.cards.push_back(testCard);

        // Advance to the next player
        pTurn = (pTurn + 1) % players.size();
        break;
    }

}

void Game::AddPlayer(const std::string& name, Player * client)
{
    UI ui;
    host->sendMessageToClient(client, "You've been added to the game.\n", false);

    gamePlayer p;
    p.client = client;
    p.name = name;
    p.cards = {}; // Initialize with an empty card list or other logic

    if (!isStarted)
    {
        players.push_back(p);
        std::cout << "Player added\n";
    }
    else
    {
        std::cout << "Game has already started. Cannot add new players.\n";
    }
}

void Game::RemovePlayer(Player * client)
{
    std::cout << "ill remove it later lol\n";
    
}


void Game::gameTick()
{
    std::cout << "--GAME TICK--\n";
    if (!isStarted) 
    {
        std::cout << "Game not started yet. Waiting...\n";
        return;
    }

    
    if (waitingForPlayer)
    {
        std::string lastResponse = host->getReponseFromClientPrompt();
        if (lastResponse != "")
        {
            if (lastResponse == "y")
            {
                UI ui;
                std::string hand = ui.getTestHand();
                host->sendMessageToClient(currentPlayerTurn->client, hand, false);
            }
            else if (lastResponse == "n") 
            {}
            else
            {
                gameTick();
                return;
            }

            
            waitingForPlayer = false;
            nextPlayer();
            std::cout << lastResponse << std::endl;
        }
    }
    else
    {
        if (currentPlayerTurn == nullptr)
        {
            std::cout << "first player setting\n";
            pTurn = -1;
            nextPlayer();
        }
        gameTick();

    }
}

void Game::nextPlayer()
{
    pTurn = ((pTurn + 1) % players.size());

    std::cout << "Next player[" << pTurn << "]"<< std::endl;
    
    currentPlayerTurn = &players.at(pTurn);

    // Choosing a player to prompt
    host->promptClient(currentPlayerTurn->client, "Want a card? ('y'/'n'): ");
    // SET waitingForPlayer to TRUE
    waitingForPlayer = true;
}

void Game::updateClients()
{

    // update all the players
    // For each client(player), send client new data (it is up to client to handle that data)
    // Ask specific client for data the host needs (like a bet,check, etc...)
    UI ui;
}
