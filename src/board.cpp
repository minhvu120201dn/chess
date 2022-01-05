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
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            this->sides[i][j] = init_sides[i][j], this->pieces[i][j] = init_pieces[i][j];    
}

Board::Board(char sides[][8], char pieces[][8], bool white_turn) : turn(white_turn) {
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            this->sides[i][j] = sides[i][j], this->pieces[i][j] = pieces[i][j];
}

Board::Board(const Board& copy) {
    *this = copy;
}

std::set<std::pair<char,char>>* Board::availMovesKing(char col, char row) {
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

    char side = sides[row][col];
    if (!king_moved[side]) {
        if (!lrook_moved[side] && sides[row][col-1] == -1 && sides[row][col-2] == -1 && sides[row][col-3])
            moveList->insert(std::make_pair(col-2,row));
        if (!rrook_moved[side] && sides[row][col+1] == -1 && sides[row][col+2] == -1)
            moveList->insert(std::make_pair(col+2,row));
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::availMovesQueen(char col, char row) {
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

std::set<std::pair<char,char>>* Board::availMovesBishop(char col, char row) {
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

std::set<std::pair<char,char>>* Board::availMovesKnight(char col, char row) {
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

std::set<std::pair<char,char>>* Board::availMovesRook(char col, char row) {
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

std::set<std::pair<char,char>>* Board::availMovesPawn(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    switch(this->sides[row][col]) {
        case WHITE: {
            if (this->pieces[row-1][col] == -1) {
                moveList->insert(std::make_pair(col,row-1));
                if (row == 6 && this->pieces[row-2][col] == -1) {
                    moveList->insert(std::make_pair(col,row-2));
                }
            }
            if (col > 0 && ((this->pieces[row-1][col-1] != -1 && this->sides[row-1][col-1] == BLACK) || this->pawn_justmoved[BLACK] == col-1))
                moveList->insert(std::make_pair(col-1,row-1));
            if (col < 7 && ((this->pieces[row-1][col+1] != -1 && this->sides[row-1][col+1] == BLACK) || this->pawn_justmoved[BLACK] == col+1))
                moveList->insert(std::make_pair(col+1,row-1));
            break;
        }
        case BLACK: {
            if (this->pieces[row+1][col] == -1) {
                moveList->insert(std::make_pair(col,row+1));
                if (row == 1 && this->pieces[row+2][col] == -1) {
                    moveList->insert(std::make_pair(col,row+2));
                }
            }
            if (col > 0 && ((this->pieces[row+1][col-1] != -1 && this->sides[row+1][col-1] == WHITE) || this->pawn_justmoved[WHITE] == col-1))
                moveList->insert(std::make_pair(col-1,row+1));
            if (col < 7 && ((this->pieces[row+1][col+1] != -1 && this->sides[row+1][col+1] == WHITE) || this->pawn_justmoved[WHITE] == col+1))
                moveList->insert(std::make_pair(col+1,row+1));
            break;
        }
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::availableMoves(char col, char row) {
    std::set<std::pair<char,char>> *ret = new std::set<std::pair<char,char>>;
    //std::cout << (int)col << (int)row << ' ' << (int)this->pieces[row][col] << ' ' << (int)this->sides[row][col] << std::endl;

    if (this->sides[row][col] != turn) return ret;
    switch(this->pieces[row][col]) {
        case KING: return this->availMovesKing(col,row); break;
        case QUEEN: return this->availMovesQueen(col,row); break;
        case BISHOP: return this->availMovesBishop(col,row); break;
        case KNIGHT: return this->availMovesKnight(col,row); break;
        case ROOK: return this->availMovesRook(col,row); break;
        case PAWN: return this->availMovesPawn(col,row); break;
    }

    return ret;
}

void Board::move(char col_from, char row_from, char col_to, char row_to) {
    // check conditions for castling and en passant
    bool side = this->sides[row_from][col_from];
    this->pawn_justmoved[side] = -1;
    switch (this->pieces[row_from][col_from]) {
        case KING: {
            king_moved[side] = true;
            if (abs(col_to - col_from) == 2) { // move rook out before castling
                if (col_to < col_from) this->move(0,row_from,3,row_to);
                else this->move(7,row_from,5,row_to);
                this->turn = !this->turn;
            }
            break;
        }
        case ROOK: {
            switch(side) {
                case WHITE: {
                    if (row_from == 7) {
                        if (col_from == 0) this->lrook_moved[WHITE] = true;
                        else if (col_from == 7) this->rrook_moved[WHITE] = true;
                    }
                    break;
                }
                case BLACK: {
                    if (row_from == 0) {
                        if (col_from == 0) this->lrook_moved[WHITE] = true;
                        else if (col_from == 7) this->rrook_moved[WHITE] = true;
                    }
                    break;
                }
            }
            break;
        }
        case PAWN: {
            if (abs(row_to - row_from) == 2)
                this->pawn_justmoved[side] = col_from;
            else if (col_to != col_from && this->sides[row_to][col_to] == -1) {
                this->sides[row_from][col_to] = -1;
                this->pieces[row_from][col_to] = -1;
            }
        }
    }

    this->sides[row_to][col_to] = side;
    this->sides[row_from][col_from] = -1;
    this->pieces[row_to][col_to] = this->pieces[row_from][col_from];
    this->pieces[row_from][col_from] = -1;
    this->turn = !this->turn;
    /*
    std::cout << "Moved from " << (int)row_from << (int)col_from << " to " << (int)row_to << (int)col_to << '\n';
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 8; ++j) {
            if (sides[i][j] == -1 && pieces[i][j] == -1) std::cout << '\t';
            else std::cout << (int)sides[i][j] << (int)pieces[i][j] << '\t';
        }
        std::cout << '\n';
    }
    std::cout << "==========================================================" << std::endl;
    */
    //std::cout << king_moved[0] << lrook_moved[0] << rrook_moved[0] << std::endl;
    //std::cout << (int)row_from << (int)col_from << ' ' << (int)row_to << (int)col_to << std::endl;
}