#include "commons.h"
#include "display.h"

#include <stack>

sf::Texture figures;
sf::Texture board_texture;

sf::RectangleShape *movingFig;
char xmov = -1, ymov = -1;
std::set<std::pair<char,char>> *availableMoves = nullptr;

DisplayedBoard::DisplayedBoard() {
    ::figures.loadFromFile("images/pieces.png");
    sf::Vector2u pieceSize = ::figures.getSize();
    pieceSize.x /= 6, pieceSize.y /= 2;
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j) {
            #define side (this->board->sides[i][j])
            #define piece (this->board->pieces[i][j])
            sf::RectangleShape *&fig = this->figs[i][j];
            if (side == -1) { fig = NULL; continue; }
            fig = new sf::RectangleShape;
            fig->setSize(sf::Vector2f(BOARD_LEN/8, BOARD_LEN/8));
            fig->setTexture(&::figures);
            fig->setTextureRect(sf::IntRect(pieceSize.x * piece, pieceSize.y * side, pieceSize.x, pieceSize.y));
            fig->setPosition(j*(BOARD_LEN/8), i*(BOARD_LEN/8));
            #undef side
            #undef piece
        }

    ::board_texture.loadFromFile("images/board.png");
    this->boardfig.setSize(sf::Vector2f(BOARD_LEN, BOARD_LEN));
    this->boardfig.setTexture(&::board_texture);
}

void DisplayedBoard::draw(sf::RenderWindow& window) {
    window.draw(this->boardfig);

    if (this->moving) {
        sf::RectangleShape cur_grid(sf::Vector2f(BOARD_LEN/8, BOARD_LEN/8));
        cur_grid.setFillColor(sf::Color::Yellow);
        cur_grid.setPosition(xmov*(BOARD_LEN/8), ymov*(BOARD_LEN/8));
        window.draw(cur_grid);

        sf::RectangleShape avail_move(sf::Vector2f(BOARD_LEN/8, BOARD_LEN/8));
        avail_move.setFillColor(sf::Color::Magenta);
        for (const std::pair<char,char> &grid : *::availableMoves) {
            avail_move.setPosition(grid.first*(BOARD_LEN/8), grid.second*(BOARD_LEN/8));
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
    x /= BOARD_LEN / 8, y /= BOARD_LEN / 8;
    if (!this->figs[y][x]) return false;
    ::movingFig = this->figs[y][x];
    ::xmov = x, ::ymov = y;
    if (::availableMoves) delete ::availableMoves;
    ::availableMoves = this->board->availableMoves(x,y);
    return true;
}

void DisplayedBoard::movingPiece(int x,int y) {
    ::movingFig->setPosition(x-BOARD_LEN/16, y-BOARD_LEN/16);
}

void DisplayedBoard::movePiece(int x, int y) {
    x /= BOARD_LEN / 8, y /= BOARD_LEN / 8;
    ::movingFig->setPosition(x*(BOARD_LEN/8), y*(BOARD_LEN/8));
    ::movingFig = nullptr;
    if (x == ::xmov && y == ::ymov) return;

    if (this->figs[y][x]) delete this->figs[y][x];
    this->figs[y][x] = this->figs[ymov][xmov];
    this->figs[ymov][xmov] = nullptr;

    this->board->sides[y][x] = this->board->sides[ymov][xmov];
    this->board->sides[ymov][xmov] = -1;

    this->board->pieces[y][x] = this->board->pieces[ymov][xmov];
    this->board->pieces[ymov][xmov] = -1;

    delete ::availableMoves;
    ::availableMoves = nullptr;
    /*
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            std::cout << (int)this->board->sides[i][j] << (int)this->board->pieces[i][j] << '\t';
        std::cout << std::endl;
    }
    */
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