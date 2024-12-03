// ui.h

#include <iostream>
#include <string>
#include "stdio.h"
#include <vector>
#include <algorithm>
#include <cstdlib>

#ifndef UI_H

#define UI_H


#define SUIT_UNSET "#"
#define FACE_UNSET "#"

#define SPADES   "\033[34m\u2660\033[0m"
#define HEARTS   "\033[91m\u2665\033[0m"
#define CLUBS    "\033[34m\u2663\033[0m"   // dark blueDeep
#define DIAMONDS "\033[91m\u2666\033[0m"   // red


#define ACE   "A"
#define TWO   "2"
#define THREE "3"
#define FOUR  "4"
#define FIVE  "5"
#define SIX   "6"
#define SEVEN "7"
#define EIGHT "8"
#define NINE  "9"
#define TEN   "10"
#define JACK  "J"
#define QUEEN "Q"
#define KING  "K"

const std::string SUITS[4] = {
    SPADES, DIAMONDS, CLUBS, HEARTS
};
const std::string FACES[13] = {
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
    TEN, JACK, QUEEN, KING
};

#define CARD_CORNER_TOP_LEFT  "\u250C"; // ┌
#define CARD_CORNER_TOP_RIGHT "\u2510"; // ┐
#define CARD_CORNER_BOT_LEFT  "\u2514"; // └
#define CARD_CORNER_BOT_RIGHT "\u2518"; // ┘
#define CARD_TOP_BOT          "\u2500"; // ─
#define CARD_LEFT_RIGHT       "\u2502"; // │

struct cardByLine 
{
    std::string firstLine;
    std::string mid1Line;
    std::string mid2Line;
    std::string mid3Line;
    std::string lastLine;

} typedef cardByLine;

class UI 
{


public:
    // Constructor
    UI();
    // Public methods
    void Start();
    void clearScreen();
    std::string promptLine(const std::string& text);
    cardByLine cardToString(std::string face, std::string suit);
    std::string getTestHand();
    std::string handToPrintable(std::vector<cardByLine> cards);

private:
    // Private
};

#endif // UI_H
