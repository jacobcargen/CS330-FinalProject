// ui.cpp
#include <iostream>
#include <string>
#include "stdio.h"
#include "ui.h"

// Constructor
UI::UI() 
{
    test();
}

// Public method
void UI::test() 
{
    std::cout << "UI Test" << std::endl;
}

std::string UI::promptLine(const std::string& text)
{
    std::cout << text;
    std::string input = "";
    std::cin >> input;
    return input;
}

void displayCard(std::string face, std::string suit)
{

}
