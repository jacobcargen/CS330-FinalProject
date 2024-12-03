// game.h
#ifndef GAME_H
#define GAME_H

#include "ui.h"
#include <string>
#include <vector>
#include "host.h"

enum game {
    POKER = 2,
} typedef game;

struct card
{
    bool shouldBePrivate = false;
    std::string face = FACE_UNSET;
    std::string suit = SUIT_UNSET;
} typedef card;

struct gamePlayer
{
    Player * client;
    int id; // not used i think
    std::string name = "no name";
    std::vector<card> cards;
    int money = 0;

} typedef gamePlayer;

class Game {
public:
    // Constructor
    Game();
    // Public methods
    void init(Host * h);
    void test();
    bool hasStarted();
    void setGameAsReady();
    void AddPlayer(const std::string& name, Player * client);
    void RemovePlayer(Player * client);
    void gameTick();

private:
    // Private
    std::vector<gamePlayer> players;
    std::vector<card> deck;
    gamePlayer * currentPlayerTurn;
    game currentGame;
    int pTurn;
    bool isStarted;
    Host * host;
    bool waitingForPlayer;

    void clearHands();
    void nextPlayer();
    void getNewDeck(std::vector<card> deck);
    void processGame();
    bool processResponse(const std::string &lastResponse);

    void drawCardToPlayer(gamePlayer * p, bool shouldBePrivate);
    std::string getDisplayedPlayer(gamePlayer * player, gamePlayer * viewingPlayer);

    void startPoker();
    void pokerGame();
    bool pokerResponse(const std::string &lastResponse);
};

#endif // GAME_H
