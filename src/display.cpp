#include "commons.h"
#include "display.h"

#include <stack>

sf::Texture figures;
sf::Texture board_texture;

sf::RectangleShape *movingFig;
char xmov = -1, ymov = -1;
std::set< std::pair<char,char> > *availableMoves = nullptr;
bool king_moving = false;
bool pawn_moving = false;

#define GRID_LEN (BOARD_LEN / 8)

DisplayedBoard::DisplayedBoard() {
    ::figures.loadFromFile("images/pieces.png");
    sf::Vector2u pieceSize = ::figures.getSize();
    pieceSize.x /= 6, pieceSize.y /= 2;
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j) {
            std::pair<char,char> info = (*this->board)[std::make_pair(i,j)];
            char side = info.first, piece = info.second;
            sf::RectangleShape *&fig = this->figs[i][j];
            if (side == -1) { fig = NULL; continue; }
            fig = new sf::RectangleShape;
            fig->setSize(sf::Vector2f(GRID_LEN, GRID_LEN));
            fig->setTexture(&::figures);
            fig->setTextureRect(sf::IntRect(pieceSize.x * piece, pieceSize.y * side, pieceSize.x, pieceSize.y));
            fig->setPosition(j*GRID_LEN, i*GRID_LEN);
        }

    ::board_texture.loadFromFile("images/board.png");
    this->boardfig.setSize(sf::Vector2f(BOARD_LEN, BOARD_LEN));
    this->boardfig.setTexture(&::board_texture);
}

void DisplayedBoard::draw(sf::RenderWindow& window) {
    window.draw(this->boardfig);

    if (this->moving) {
        sf::RectangleShape cur_grid(sf::Vector2f(GRID_LEN, GRID_LEN));
        cur_grid.setFillColor(sf::Color::Yellow);
        cur_grid.setPosition(xmov*GRID_LEN, ymov*GRID_LEN);
        window.draw(cur_grid);

        sf::RectangleShape avail_move(sf::Vector2f(GRID_LEN, GRID_LEN));
        avail_move.setFillColor(sf::Color::Magenta);
        for (const std::pair<char,char> &grid : *::availableMoves) {
            avail_move.setPosition(grid.first*GRID_LEN, grid.second*GRID_LEN);
            window.draw(avail_move);
        }
    }

    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            if (this->figs[i][j] && this->figs[i][j] != ::movingFig)
                window.draw(*this->figs[i][j]);
    if (::movingFig)
        window.draw(*::movingFig);
}

bool DisplayedBoard::beginMove(int x, int y) {
    x /= GRID_LEN, y /= GRID_LEN;
    if (!this->figs[y][x]) return false;
    ::movingFig = this->figs[y][x];
    ::xmov = x, ::ymov = y;
    ::availableMoves = this->board->availableMoves(x,y);
    switch((*this->board)[std::make_pair(y,x)].second) {
        case KING: ::king_moving = true; break;
        case PAWN: ::pawn_moving = true; break;
    }
    return true;
}

void DisplayedBoard::movingPiece(int x,int y) {
    ::movingFig->setPosition(x-GRID_LEN/2, y-GRID_LEN/2);
}

void DisplayedBoard::movePiece(int x, int y) {
    bool king_moving = ::king_moving; ::king_moving = false;
    bool pawn_moving = ::pawn_moving; ::pawn_moving = false;
    x /= GRID_LEN, y /= GRID_LEN;
    ::movingFig->setPosition(xmov*GRID_LEN, ymov*GRID_LEN);
    if (x == ::xmov && y == ::ymov) {
        ::movingFig = nullptr;
        delete ::availableMoves;
        ::availableMoves = nullptr;
        return;
    }
#ifndef DEBUG
    if (!::availableMoves->count(std::make_pair(x,y))) {
        ::movingFig = nullptr;
        delete ::availableMoves;
        ::availableMoves = nullptr;
        return;
    }
#endif

    if (king_moving && abs(x-xmov)==2) {
        if (x < xmov) {
            this->figs[y][0]->setPosition(3*GRID_LEN, y*GRID_LEN);
            this->figs[y][3] = this->figs[y][0];
            this->figs[y][0] = nullptr;
        }
        else {
            this->figs[y][7]->setPosition(5*GRID_LEN, y*GRID_LEN);
            this->figs[y][5] = this->figs[y][7];
            this->figs[y][7] = nullptr;
        }
    }
    if (pawn_moving && x!=xmov && !this->figs[y][x]) { // en passant
        delete this->figs[ymov][x];
        this->figs[ymov][x] = nullptr;
    }

    ::movingFig->setPosition(x*GRID_LEN, y*GRID_LEN);
    ::movingFig = nullptr;
    if (this->figs[y][x]) delete this->figs[y][x];
    this->figs[y][x] = this->figs[ymov][xmov];
    this->figs[ymov][xmov] = nullptr;

    this->board->move(xmov,ymov,x,y);

    delete ::availableMoves;
    ::availableMoves = nullptr;
}

DisplayedBoard::~DisplayedBoard() {
    if (::movingFig) delete ::movingFig;
    if (::availableMoves) delete ::availableMoves;

    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            if (this->figs[i][j])
                delete this->figs[i][j];
    delete this->board;
}