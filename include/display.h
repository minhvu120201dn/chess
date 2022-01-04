#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <SFML/Graphics.hpp>
#include "board.h"

class DisplayedBoard {
    sf::RectangleShape boardfig;
    sf::RectangleShape *figs[8][8];
    Board board;
public:
    bool moving = false;
    DisplayedBoard();
    void draw(sf::RenderWindow&);
    bool beginMove(int,int);
    void movingPiece(int,int);
    void movePiece(int,int);
};

#endif