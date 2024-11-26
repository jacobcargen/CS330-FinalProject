// game.h
#ifndef GAME_H
#define GAME_H

#include "ui.h"
#include <string>
#include <vector>
#include "host.h"


struct card
{
    std::string face = FACE_UNSET;
    std::string suit = SUIT_UNSET;
} typedef card;

struct gamePlayer
{
    Player * client;
    int id;
    std::string name;
    std::vector<card> cards;
    int money;

} typedef gamePlayer;

class Game {
public:
    // Constructor
    Game();
    // Public methods
    void init(Host * h);
    void test();
    bool hasStarted();
    void dealCardsTestGameExample();
    void setGameAsReady();
    void AddPlayer(const std::string& name, Player * client);
    void RemovePlayer(Player * client);
    void gameTick();

    void nextPlayer();

private:
    // Private
    bool isStarted;
    void updateClients();
};

#endif // GAME_H
