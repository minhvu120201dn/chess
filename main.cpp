#include <SFML/Graphics.hpp>
#include <iostream>

#include "commons.h"
#include "display.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(BOARD_LEN, BOARD_LEN), "Chess");

    DisplayedBoard board;
    int mouse_x, mouse_y;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    float x = event.mouseButton.x, y = event.mouseButton.y;
                    if (board.beginMove(x,y))
                        board.moving = true;
                    break;
                }
                case sf::Event::MouseMoved: {
                    if (board.moving) {
                        mouse_x = event.mouseMove.x;
                        mouse_y = event.mouseMove.y;
                        if (mouse_x < 0) mouse_x = 0;
                        if (mouse_x > BOARD_LEN) mouse_x = BOARD_LEN-1;
                        if (mouse_y < 0) mouse_y = 0;
                        if (mouse_y > BOARD_LEN) mouse_y = BOARD_LEN-1;
                        board.movingPiece(mouse_x, mouse_y);
                    }
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    board.moving = false;
                    board.movePiece(mouse_x, mouse_y);
                    //board.pieces[WHITE][KING].setPosition((mouse_x/(BOARD_LEN/8))*(BOARD_LEN/8), (mouse_y/(BOARD_LEN/8))*(BOARD_LEN/8));
                    break;
                }
            }
        }
        window.clear();
        board.draw(window);
        window.display();
    }
}