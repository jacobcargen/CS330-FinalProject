// game.cpp
#include <iostream>
#include "game.h"
#include "message_helper.h"
#include "host.h"
#include "game.h"
#include "ui.h"
#include <vector>

// Store players here (TYPE 'gamePlayer' (game player) not to be confused with TYPE 'Player'(CLIENT))
std::vector<gamePlayer> players;
std::vector<card> deck;
gamePlayer * currentPlayerTurn = nullptr;
game currentGame = POKER;
int pTurn = 0;
bool isStarted = false;
Host * host;
bool waitingForPlayer = false;

// Constructor
Game::Game() { }

void Game::init(Host * h)
{
    host = h;
}
bool Game::hasStarted()
{
    return isStarted;
}
void Game::setGameAsReady()
{
    isStarted = true;
    waitingForPlayer = false;
    // Decide game here maybe
    startPoker();

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
    if (!isStarted) 
    {
        std::cout << "Game not started yet. Waiting...\n";
        return;
    }
    
    std::cout << "--GAME TICK--\n";

    // Handle waiting for player response
    while (waitingForPlayer)
    {
        std::string lastResponse = host->getReponseFromClientPrompt();
        
        if (!lastResponse.empty()) 
        {
            std::cout << "Received response: " << lastResponse << std::endl;

            // Process the response
            if (processResponse(lastResponse))
            {
                std::cout << "Valid response processed, moving to next player.\n";
                waitingForPlayer = false;
                host->promptComplete();
            }
            else
            {
                // FIX ME
                host->reprompt(currentPlayerTurn->client);
            }
        }
        else 
        {
            std::cout << "Waiting for response...\n";
            sleep(1);
        }
    }
    // Response success, proceed VVV
    // Not waiting for player anymore VVV
    processGame();
}
void Game::nextPlayer()
{
    pTurn = ((pTurn + 1) % players.size());

    std::cout << "Next player[" << pTurn << "]"<< std::endl;
    
    currentPlayerTurn = &players.at(pTurn);
    if (currentPlayerTurn == nullptr || currentPlayerTurn->client == nullptr)
        std::cerr << "NULL PLAYER!!!" << std::endl;
    // SET waitingForPlayer to TRUE
    waitingForPlayer = true;
}
void Game::getNewDeck(std::vector<card> deck)
{
    deck.clear();
    for (std::string face : FACES)
    {
        for (std::string suit : SUITS)
        {
            card c;
            c.face = face;
            c.suit = suit;
            deck.push_back(c);
        }
    }
    return;
}
void Game::drawCardToPlayer(gamePlayer * p, bool shouldBePrivate) 
{
    if (deck.empty()) 
        return;
    int i = std::rand() % deck.size();
    card c;
    c = deck[i];
    c.shouldBePrivate = shouldBePrivate;
    std::cout << "!!!!!card is " << c.face << c.suit << std::endl;
    p->cards.push_back(c);
    deck.erase(deck.begin() + i);
}
void Game::clearHands()
{
    // Clear hands
    for (gamePlayer& p : players)
    {
        p.cards.clear();
    }
}
bool Game::processResponse(const std::string &lastResponse)
{
    std::cout << "|||processResponse|||" << std::endl;
    switch (currentGame)
    {
    case POKER:
        return pokerResponse(lastResponse);
    
    default:
        return false;
        // error
    }
}
void Game::processGame()
{
    std::cout << "|||processGame|||" << std::endl;
    switch (currentGame)
    {
    case POKER:
        pokerGame();
        break;
    
    default:
        break;
    }
}

std::string Game::getDisplayedPlayer(gamePlayer * player, gamePlayer * viewingPlayer)
{
    UI ui;
    std::string str = "";

    str += player->name;
    str += "\t\t$";
    str += std::to_string(player->money);
    str += "\n";

    std::vector<cardByLine> cardsStr;
    for (card c : player->cards)
    {
        cardByLine cblCard;
        if (c.shouldBePrivate && viewingPlayer != player)
        {
            // dont show
            cblCard = ui.cardToString(FACE_UNSET, SUIT_UNSET);
        }
        else
        {
            cblCard = ui.cardToString(c.face, c.suit);
        }
        cardsStr.push_back(cblCard);
    }
    std::string hand = ui.handToPrintable(cardsStr);
    str += hand;
    return str;
}

#pragma region POKER

// Poker variables

void Game::startPoker()
{
    currentGame = POKER;

    for (gamePlayer& p : players)
    {
        p.money = 500;
    }

    deck.clear();
    getNewDeck(deck);

    // Set up the first playedr
    std::cout << "First player setting\n";
    pTurn = -1;
}
bool Game::pokerResponse(const std::string &lastResponse)
{
    std::cout << "|||pokerResponse|||" << std::endl;
    if (lastResponse == "y")
    {
        drawCardToPlayer(currentPlayerTurn, false);
        return true;
    }
    else if (lastResponse == "n")
        return true;
    
    // All else not valid
    return false;
}
void Game::pokerGame()
{
    std::cout << "|||pokerGame|||" << std::endl;
    UI ui;
    
    // Update hands for all
    
    for (gamePlayer& player : players)
    {
        std::string str = "";
        for (gamePlayer& otherPlayer : players)
        {
            if (&player == &otherPlayer)
                str += "[YOU]";
            std::string s = getDisplayedPlayer(&otherPlayer, &player);
            str += s;
        }
        host->sendMessageToClient(player.client, str, false);
        std::cout << std::to_string(player.cards.at(0));
    }
    // Next player turn
    nextPlayer();
    // Prompt currentPlayerTurn
    
    host->promptClient(currentPlayerTurn->client, "Want a card? ('y'/'n'): ");

    // Or waiting for dealing etc.
    // 
    //give cards display changes etc



}

#pragma endregion
