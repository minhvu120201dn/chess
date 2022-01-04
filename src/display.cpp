#include "commons.h"
#include "display.h"

#include <iostream>

sf::Texture figures;
sf::Texture board_texture;

sf::Vector2u pieceSize;
sf::RectangleShape *movingFig;
int xmov = -1, ymov = -1;

DisplayedBoard::DisplayedBoard() {
    ::figures.loadFromFile("images/pieces.png");
    pieceSize = ::figures.getSize();
    pieceSize.x /= 6, pieceSize.y /= 2;
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j) {
            #define side (this->board.sides[i][j])
            #define piece (this->board.pieces[i][j])
            sf::RectangleShape *&fig = this->figs[i][j];
            if (side == -1) { fig = NULL; continue; }
            fig = new sf::RectangleShape;
            fig->setSize(sf::Vector2f(BOARD_LEN/8, BOARD_LEN/8));
            fig->setTexture(&::figures);
            fig->setTextureRect(sf::IntRect(pieceSize.x * piece, pieceSize.y * side, pieceSize.x, pieceSize.y));
            fig->setPosition(j * (BOARD_LEN/8), i * (BOARD_LEN/8));
            #undef side
            #undef piece
        }

    ::board_texture.loadFromFile("images/board.png");
    this->boardfig.setSize(sf::Vector2f(BOARD_LEN, BOARD_LEN));
    this->boardfig.setTexture(&::board_texture);
}

void DisplayedBoard::draw(sf::RenderWindow& window) {
    window.draw(this->boardfig);

    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            if (this->figs[i][j] && this->figs[i][j] != ::movingFig)
                window.draw(*this->figs[i][j]);
    if (::movingFig)
        window.draw(*::movingFig);
}

bool DisplayedBoard::beginMove(int x, int y) {
    x /= BOARD_LEN / 8, y /= BOARD_LEN / 8;
    //std::cout << this->figs[y][x] << std::endl;
    if (this->figs[y][x] == nullptr) return false;
    ::movingFig = this->figs[y][x];
    ::xmov = x, ::ymov = y;
    return true;
}

void DisplayedBoard::movingPiece(int x,int y) {
    ::movingFig->setPosition(x-BOARD_LEN/16, y-BOARD_LEN/16);
}

void DisplayedBoard::movePiece(int x, int y) {
    x /= BOARD_LEN / 8, y /= BOARD_LEN / 8;
    ::movingFig->setPosition(x*(BOARD_LEN/8), y*(BOARD_LEN/8));
    ::movingFig = nullptr;
    if (x == xmov && y == ymov) return;

    if (this->figs[y][x]) delete this->figs[y][x];

    this->figs[y][x] = this->figs[ymov][xmov];
    //this->board.pieces[y][x] = this->board.pieces[ymov][xmov];
    //this->board.sides[y][x] = this->board.sides[ymov][xmov];

    this->figs[ymov][xmov] = nullptr;
    //this->board.pieces[ymov][xmov] = -1;
    //this->board.sides[ymov][xmov] = -1;
}