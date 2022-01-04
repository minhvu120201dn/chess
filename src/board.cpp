#include "commons.h"
#include "board.h"

char init_sides[8][8] = { { 1, 1, 1, 1, 1, 1, 1, 1},
                          { 1, 1, 1, 1, 1, 1, 1, 1},
                          {-1,-1,-1,-1,-1,-1,-1,-1},
                          {-1,-1,-1,-1,-1,-1,-1,-1},
                          {-1,-1,-1,-1,-1,-1,-1,-1},
                          {-1,-1,-1,-1,-1,-1,-1,-1},
                          { 0, 0, 0, 0, 0, 0, 0, 0},
                          { 0, 0, 0, 0, 0, 0, 0, 0} };

char init_pieces[8][8] = { { 4, 3, 2, 1, 0, 2, 3, 4},
                           { 5, 5, 5, 5, 5, 5, 5, 5},
                           {-1,-1,-1,-1,-1,-1,-1,-1},
                           {-1,-1,-1,-1,-1,-1,-1,-1},
                           {-1,-1,-1,-1,-1,-1,-1,-1},
                           {-1,-1,-1,-1,-1,-1,-1,-1},
                           { 5, 5, 5, 5, 5, 5, 5, 5},
                           { 4, 3, 2, 1, 0, 2, 3, 4} };

Board::Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            this->sides[i][j] = init_sides[i][j], this->pieces[i][j] = init_pieces[i][j];    
}

Board::Board(char sides[][8], char pieces[][8], bool white_turn) : white_turn(white_turn) {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            this->sides[i][j] = sides[i][j], this->pieces[i][j] = pieces[i][j];
}

std::set<std::pair<char,char>>* Board::moveKing(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    if (col > 0) {
        if (row > 0 && !this->sameSide(col,row,col-1,row-1)) moveList->insert(std::make_pair(col-1,row-1));
        if (!this->sameSide(col,row,col-1,row)) moveList->insert(std::make_pair(col-1,row));
        if (row < 7 && !this->sameSide(col,row,col-1,row+1)) moveList->insert(std::make_pair(col-1,row+1));
    }

    if (row > 0 && !this->sameSide(col,row,col,row-1)) moveList->insert(std::make_pair(col,row-1));
    if (row < 7 && !this->sameSide(col,row,col,row+1)) moveList->insert(std::make_pair(col,row+1));

    if (col < 7) {
        if (row > 0 && !this->sameSide(col,row,col+1,row-1)) moveList->insert(std::make_pair(col+1,row-1));
        if (!this->sameSide(col,row,col+1,row)) moveList->insert(std::make_pair(col+1,row));
        if (row < 7 && !this->sameSide(col,row,col+1,row+1)) moveList->insert(std::make_pair(col+1,row+1));
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::moveQueen(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    for (char vert = col + 1, hori = row + 1; vert <= 7 && hori <= 7; ++vert, ++hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col + 1, hori = row - 1; vert <= 7 && hori >= 0; ++vert, --hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col - 1, hori = row + 1; vert >= 0 && hori <= 7; --vert, ++hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col - 1, hori = row - 1; vert >= 0 && hori >= 0; --vert, --hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col + 1; vert <= 7; ++vert) {
        if (this->sameSide(col,row,vert,row)) break;
        moveList->insert(std::make_pair(vert,row));
        if (!this->empty(vert,row)) break;
    }

    for (char vert = col - 1; vert >= 0; --vert) {
        if (this->sameSide(col,row,vert,row)) break;
        moveList->insert(std::make_pair(vert,row));
        if (!this->empty(vert,row)) break;
    }

    for (char hori = row + 1; hori <= 7; ++hori) {
        if (this->sameSide(col,row,col,hori)) break;
        moveList->insert(std::make_pair(col,hori));
        if (!this->empty(col,hori)) break;
    }

    for (char hori = row - 1; hori >= 0; --hori) {
        if (this->sameSide(col,row,col,hori)) break;
        moveList->insert(std::make_pair(col,hori));
        if (!this->empty(col,hori)) break;
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::moveBishop(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    for (char vert = col + 1, hori = row + 1; vert <= 7 && hori <= 7; ++vert, ++hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col + 1, hori = row - 1; vert <= 7 && hori >= 0; ++vert, --hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col - 1, hori = row + 1; vert >= 0 && hori <= 7; --vert, ++hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    for (char vert = col - 1, hori = row - 1; vert >= 0 && hori >= 0; --vert, --hori) {
        if (this->sameSide(col,row,vert,hori)) break;
        moveList->insert(std::make_pair(vert,hori));
        if (!this->empty(vert,hori)) break;
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::moveKnight(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    if (col > 1) {
        if (row > 0 && !this->sameSide(col,row,col-2,row-1)) moveList->insert(std::make_pair(col-2,row-1));
        if (row < 7 && !this->sameSide(col,row,col-2,row+1)) moveList->insert(std::make_pair(col-2,row+1));
    }

    if (col > 0) {
        if (row > 1 && !this->sameSide(col,row,col-1,row-2)) moveList->insert(std::make_pair(col-1,row-2));
        if (row < 6 && !this->sameSide(col,row,col-1,row+2)) moveList->insert(std::make_pair(col-1,row+2));
    }

    if (col < 7) {
        if (row > 1 && !this->sameSide(col,row,col+1,row-2)) moveList->insert(std::make_pair(col+1,row-2));
        if (row < 6 && !this->sameSide(col,row,col+1,row+2)) moveList->insert(std::make_pair(col+1,row+2));
    }

    if (col < 6) {
        if (row > 0 && !this->sameSide(col,row,col+2,row-1)) moveList->insert(std::make_pair(col+2,row-1));
        if (row < 7 && !this->sameSide(col,row,col+2,row+1)) moveList->insert(std::make_pair(col+2,row+1));
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::moveRook(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    for (char vert = col + 1; vert <= 7; ++vert) {
        if (this->sameSide(col,row,vert,row)) break;
        moveList->insert(std::make_pair(vert,row));
        if (!this->empty(vert,row)) break;
    }

    for (char vert = col - 1; vert >= 0; --vert) {
        if (this->sameSide(col,row,vert,row)) break;
        moveList->insert(std::make_pair(vert,row));
        if (!this->empty(vert,row)) break;
    }

    for (char hori = row + 1; hori <= 7; ++hori) {
        if (this->sameSide(col,row,col,hori)) break;
        moveList->insert(std::make_pair(col,hori));
        if (!this->empty(col,hori)) break;
    }

    for (char hori = row - 1; hori >= 0; --hori) {
        if (this->sameSide(col,row,col,hori)) break;
        moveList->insert(std::make_pair(col,hori));
        if (!this->empty(col,hori)) break;
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::movePawn(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;
    return moveList;
}

std::set<std::pair<char,char>>* Board::availableMoves(char x, char y) {
    std::set<std::pair<char,char>> *ret = new std::set<std::pair<char,char>>;
    //std::cout << (int)x << (int)y << ' ' << (int)this->pieces[y][x] << ' ' << (int)this->sides[y][x] << std::endl;

    switch(this->pieces[y][x]) {
        case KING: return this->moveKing(x,y); break;
        case QUEEN: return this->moveQueen(x,y); break;
        case BISHOP: return this->moveBishop(x,y); break;
        case KNIGHT: return this->moveKnight(x,y); break;
        case ROOK: return this->moveRook(x,y); break;
        case PAWN: return this->movePawn(x,y); break;
    }

    return ret;
}