#ifndef __BOARD_H__
#define __BOARD_H__

class Board {
    char sides[8][8];
    char pieces[8][8];
    bool white_turn = true;
    bool wking_moved = false, wlrook_moved = false, wrrook_moved = false;
    bool bking_moved = false, blrook_moved = false, brrook_moved = false;
private:
    inline bool sameSide(char col1, char row1, char col2, char row2) {
        return this->sides[row1][col1] == this->sides[row2][col2];
    }
    inline bool empty(char col, char row) {
        return this->sides[row][col] == -1;
    }
    std::set<std::pair<char,char>>* moveKing(char,char);
    std::set<std::pair<char,char>>* moveQueen(char,char);
    std::set<std::pair<char,char>>* moveBishop(char,char);
    std::set<std::pair<char,char>>* moveKnight(char,char);
    std::set<std::pair<char,char>>* moveRook(char,char);
    std::set<std::pair<char,char>>* movePawn(char,char);
public:
    Board();
    Board(char(*)[8],char(*)[8],bool);
    void showAvailableMoves(char,char);
    std::set<std::pair<char,char>>* availableMoves(char,char);
    friend class DisplayedBoard;
};

#endif