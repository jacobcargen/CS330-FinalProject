// game.h
#ifndef GAME_H
#define GAME_H

#define SUIT_UNSET "#"
#define FACE_UNSET '#'

#define SPADES   "\u2660"
#define HEARTS   "\u2665"
#define CLUBS    "\u2663"
#define DIAMONDS "\u2666"

#define ACE   'A'
#define TWO   '2'
#define THREE '3'
#define FOUR  '4'
#define FIVE  '5'
#define SIX   '6'
#define SEVEN '7'
#define EIGHT '8'
#define NINE  '9'
#define TEN   '10'
#define JACK  'J'
#define QUEEN 'Q'
#define KING  'K'

struct card
{
    char face = FACE_UNSET;
    std::string suit = SUIT_UNSET;
} typedef card;

struct player
{
    int id;
    std::string name;
    card * cards;
    int money;

} typedef player;

class Game {
public:
    // Constructor
    Game();
    // Public methods
    void test();

private:
    // Private
    void Game::waitForReady();
    void Game::gameLoop();
    void Game::updateClients();
};

#endif // GAME_H
