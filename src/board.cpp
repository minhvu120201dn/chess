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

bool cover_king = false;
std::set<std::pair<char,char>> cover;

Board::Board() {
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            this->sides[i][j] = init_sides[i][j], this->pieces[i][j] = init_pieces[i][j];
    this->king_row[WHITE] = 7, this->king_col[WHITE] = 4;
    this->king_row[BLACK] = 0, this->king_col[BLACK] = 4;
}

Board::Board(const Board& copy) {
    *this = copy;
}

bool Board::checkKingSafe(char col, char row, bool side) {
    std::set<std::pair<char,char>> *temp;
    char actual_side = this->sides[row][col];
    this->sides[row][col] = side;
    bool ret = true;

    temp = this->availMovesKnight(col,row);
    for (std::pair<char,char> grid : *temp) {
        if (this->sides[grid.second][grid.first] == !side &&
                this->pieces[grid.second][grid.first] == KNIGHT) {
            this->sides[row][col] = actual_side;
            if (!::cover_king) return false;
            else {
                if (ret) {
                    ret = false;
                    cover.insert(std::make_pair(grid.first,grid.second));
                }
                else {
                    ::cover.clear();
                    return false;
                }
            }
        }
    }
    delete temp;

    temp = this->availMovesRook(col,row);
    for (std::pair<char,char> grid : *temp) {
        if (this->sides[grid.second][grid.first] == !side &&
                (this->pieces[grid.second][grid.first] == ROOK ||
                 this->pieces[grid.second][grid.first] == QUEEN)) {
            this->sides[row][col] = actual_side;
            if (!::cover_king) return false;
            else {
                if (ret) {
                    ret = false;
                    if (grid.second < row) {
                        for (int hori = grid.second + 1; hori <= row; ++hori)
                            ::cover.insert(std::make_pair(col,hori));
                    }
                    else if (grid.second > row) {
                        for (int hori = grid.second - 1; hori >= row; --hori)
                            ::cover.insert(std::make_pair(col,hori));
                    }
                    else if (grid.first < col) {
                        for (int vert = grid.first + 1; vert <= col; ++vert)
                            ::cover.insert(std::make_pair(vert,row));
                    }
                    else if (grid.first > col) {
                        for (int vert = grid.first - 1; vert >= col; --vert)
                            ::cover.insert(std::make_pair(vert,row));
                    }
                }
                else {
                    ::cover.clear();
                    return false;
                }
            }
        }
    }
    delete temp;

    temp = this->availMovesBishop(col,row);
    for (std::pair<char,char> grid : *temp) {
        if (this->sides[grid.second][grid.first] == !side &&
                (this->pieces[grid.second][grid.first] == BISHOP ||
                 this->pieces[grid.second][grid.first] == QUEEN)) {
            this->sides[row][col] = actual_side;
            if (!::cover_king) return false;
            else {
                if (ret) {
                    ret = false;
                    if (grid.second < row && grid.first < col) {
                        for (int hori = grid.second + 1, vert = grid.first + 1; hori <= row && vert <= col; ++hori, ++vert)
                            ::cover.insert(std::make_pair(vert,hori));
                    }
                    else if (grid.second > row && grid.first < col) {
                        for (int hori = grid.second - 1, vert = grid.first + 1; hori >= row && vert <= col; --hori, ++vert)
                            ::cover.insert(std::make_pair(vert,hori));
                    }
                    else if (grid.second < row && grid.first > col) {
                        for (int hori = grid.second + 1, vert = grid.first - 1; hori <= row && vert >= col; ++hori, --vert)
                            ::cover.insert(std::make_pair(vert,hori));
                    }
                    else if (grid.second > row && grid.first > col) {
                        for (int hori = grid.second - 1, vert = grid.first - 1; hori >= row && vert >= col; --hori, --vert)
                            ::cover.insert(std::make_pair(vert,hori));
                    }
                }
                else {
                    ::cover.clear();
                    return false;
                }
            }
        }
    }
    delete temp;

    this->sides[row][col] = actual_side;

    switch(side) {
        case WHITE: {
            if (row > 0 && col > 0) {
                if (sides[row-1][col-1] == BLACK && pieces[row-1][col-1] == PAWN) {
                    if (!::cover_king) return false;
                    else {
                        if (ret) {
                            cover.insert(std::make_pair(col-1,row-1));
                            ret = false;
                        }
                        else {
                            ::cover.clear();
                            return false;
                        }
                    }
                }
                //std::cout << (int)row-1 << ' ' << (int)col-1 << std::endl;
            }
            if (row > 0 && col < 7) {
                if (sides[row-1][col+1] == BLACK && pieces[row-1][col+1] == PAWN) {
                    if (!::cover_king) return false;
                    else {
                        if (ret) {
                            cover.insert(std::make_pair(col+1,row-1));
                            ret = false;
                        }
                        else {
                            ::cover.clear();
                            return false;
                        }
                    }
                }
                //std::cout << (int)row-1 << ' ' << (int)col+1 << std::endl;
            }
            break;
        }
        case BLACK: {
            if (row < 7 && col > 0) {
                if (sides[row+1][col-1] == WHITE && pieces[row+1][col-1] == PAWN) {
                    if (!::cover_king) return false;
                    else {
                        if (ret) {
                            cover.insert(std::make_pair(col-1,row+1));
                            ret = false;
                        }
                        else {
                            ::cover.clear();
                            return false;
                        }
                    }
                }
                //std::cout << (int)row+1 << ' ' << (int)col-1 << std::endl;
            }
            if (row < 7 && col < 7) {
                if (sides[row+1][col+1] == WHITE && pieces[row+1][col+1] == PAWN) {
                    if (!::cover_king) return false;
                    else {
                        if (ret) {
                            cover.insert(std::make_pair(col+1,row+1));
                            ret = false;
                        }
                        else {
                            ::cover.clear();
                            return false;
                        }
                    }
                }
                //std::cout << (int)row+1 << ' ' << (int)col+1 << std::endl;
            }
            break;
        }
    }

    return ret;
}

std::set<std::pair<char,char>>* Board::availMovesKing(char col, char row) {
    std::set<std::pair<char,char>> *moveList = new std::set<std::pair<char,char>>;

    bool leftsafe_king = true, rightsafe_king = true;
    char myside = this->sides[row][col];
    //std::cout << (int)row << ' ' << (int)col << ' ' << (int)myside << std::endl;
    if (col > 0) {
        if (row > 0 && !this->sameSide(col,row,col-1,row-1)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col-1,row-1,myside))
                moveList->insert(std::make_pair(col-1,row-1));
            this->sides[row][col] = myside;
        }

        if (!this->sameSide(col,row,col-1,row)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col-1,row,myside))
                moveList->insert(std::make_pair(col-1,row));
            else leftsafe_king = false;
            this->sides[row][col] = myside;
        }

        if (row < 7 && !this->sameSide(col,row,col-1,row+1)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col-1,row+1,myside))
                moveList->insert(std::make_pair(col-1,row+1));
            this->sides[row][col] = myside;
        }
    }

    if (row > 0 && !this->sameSide(col,row,col,row-1)) {
        this->sides[row][col] = -1;
        if (this->checkKingSafe(col,row-1,myside))
            moveList->insert(std::make_pair(col,row-1));
        this->sides[row][col] = myside;
    }

    if (row < 7 && !this->sameSide(col,row,col,row+1)) {
        this->sides[row][col] = -1;
        if (this->checkKingSafe(col,row+1,myside))
            moveList->insert(std::make_pair(col,row+1));
        this->sides[row][col] = myside;
    }

    if (col < 7) {
        if (row > 0 && !this->sameSide(col,row,col+1,row-1)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col+1,row-1,myside))
                moveList->insert(std::make_pair(col+1,row-1));
            this->sides[row][col] = myside;
        }

        if (!this->sameSide(col,row,col+1,row)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col+1,row,myside))
                moveList->insert(std::make_pair(col+1,row));
            else rightsafe_king = false;
            this->sides[row][col] = myside;
        }

        if (row < 7 && !this->sameSide(col,row,col+1,row+1)) {
            this->sides[row][col] = -1;
            if (this->checkKingSafe(col+1,row+1,myside))
                moveList->insert(std::make_pair(col+1,row+1));
            this->sides[row][col] = myside;
        }
    }

    // castling
    if (!king_moved[myside]) {
        if (!lrook_moved[myside] && sides[row][col-1] == -1 && sides[row][col-2] == -1 && sides[row][col-3] == -1) {
            this->sides[row][col] = -1;
            if (leftsafe_king && this->checkKingSafe(col-2,row,myside))
                moveList->insert(std::make_pair(col-2,row));
            this->sides[row][col] = myside;
        }

        if (!rrook_moved[myside] && sides[row][col+1] == -1 && sides[row][col+2] == -1) {
            this->sides[row][col] = -1;
            if (rightsafe_king && this->checkKingSafe(col+2,row,myside))
                moveList->insert(std::make_pair(col+2,row));
            this->sides[row][col] = myside;
        }
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
            if (col > 0 && ((this->pieces[row-1][col-1] != -1 && this->sides[row-1][col-1] == BLACK) || (row == 3 && this->pawn_justmoved[BLACK] == col-1)))
                moveList->insert(std::make_pair(col-1,row-1));
            if (col < 7 && ((this->pieces[row-1][col+1] != -1 && this->sides[row-1][col+1] == BLACK) || (row == 3 && this->pawn_justmoved[BLACK] == col+1)))
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
            if (col > 0 && ((this->pieces[row+1][col-1] != -1 && this->sides[row+1][col-1] == WHITE) || (row == 4 && this->pawn_justmoved[WHITE] == col-1)))
                moveList->insert(std::make_pair(col-1,row+1));
            if (col < 7 && ((this->pieces[row+1][col+1] != -1 && this->sides[row+1][col+1] == WHITE) || (row == 4 && this->pawn_justmoved[WHITE] == col+1)))
                moveList->insert(std::make_pair(col+1,row+1));
            break;
        }
    }

    return moveList;
}

std::set<std::pair<char,char>>* Board::availableMoves(char col, char row) {
    if (this->sides[row][col] != turn) return new std::set<std::pair<char,char>>;

    std::set<std::pair<char,char>> *availMoves;
    switch(this->pieces[row][col]) {
        case KING: availMoves = this->availMovesKing(col,row); break;
        case QUEEN: availMoves = this->availMovesQueen(col,row); break;
        case BISHOP: availMoves = this->availMovesBishop(col,row); break;
        case KNIGHT: availMoves = this->availMovesKnight(col,row); break;
        case ROOK: availMoves = this->availMovesRook(col,row); break;
        case PAWN: availMoves = this->availMovesPawn(col,row); break;
        default: availMoves = new std::set<std::pair<char,char>>;
    }

    ::cover_king = true;
    if (!this->ourKingSafe()) {
        if (this->pieces[row][col] != KING) {
            std::set<std::pair<char,char>>::iterator it = availMoves->begin();
            while (it != availMoves->end()) {
                if (!::cover.count(*it)) availMoves->erase(it++);
                else ++it;
            }
        }
    }
    ::cover_king = false;

    return availMoves;
}

void Board::move(char col_from, char row_from, char col_to, char row_to) {
    // check conditions for castling and en passant
    bool side = this->sides[row_from][col_from];
    this->pawn_justmoved[side] = -1;
    switch (this->pieces[row_from][col_from]) {
        case KING: {
            king_moved[side] = true;
            this->king_row[side] = row_to;
            this->king_col[side] = col_to;
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
                        if (col_from == 0) this->lrook_moved[BLACK] = true;
                        else if (col_from == 7) this->rrook_moved[BLACK] = true;
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