#ifndef __BOARD_H__
#define __BOARD_H__

class Board {
    char sides[8][8];
    char pieces[8][8];
    bool turn = WHITE;
    bool king_moved[2] = {false}, lrook_moved[2] = {false}, rrook_moved[2] = {false};
    char pawn_justmoved[2] = {-1};
private:
    inline bool sameSide(char col1, char row1, char col2, char row2) {
        return this->sides[row1][col1] == this->sides[row2][col2];
    }
    inline bool empty(char col, char row) {
        return this->sides[row][col] == -1;
    }
    bool checkKingSafe(char,char,bool);
    std::set<std::pair<char,char>>* availMovesKing(char,char);
    std::set<std::pair<char,char>>* availMovesQueen(char,char);
    std::set<std::pair<char,char>>* availMovesBishop(char,char);
    std::set<std::pair<char,char>>* availMovesKnight(char,char);
    std::set<std::pair<char,char>>* availMovesRook(char,char);
    std::set<std::pair<char,char>>* availMovesPawn(char,char);
public:
    Board();
    Board(char(*)[8],char(*)[8],bool);
    Board(const Board&);
    inline Board& operator=(const Board&);
    inline std::pair<char,char> operator[](std::pair<char,char> loc) {
        return std::make_pair(sides[loc.first][loc.second], pieces[loc.first][loc.second]);
    }
    std::set<std::pair<char,char>>* availableMoves(char,char);
    void move(char,char,char,char);
    friend class DisplayedBoard;
};

inline Board& Board::operator=(const Board& copy) {
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            this->sides[i][j] = copy.sides[i][j], this->pieces[i][j] = copy.sides[i][j];
    this->turn = copy.turn;
    this->king_moved[0] = copy.king_moved[0], this->king_moved[1] = copy.king_moved[1];
    this->lrook_moved[0] = copy.lrook_moved[0], this->lrook_moved[1] = copy.lrook_moved[1];
    this->rrook_moved[0] = copy.rrook_moved[0], this->rrook_moved[1] = copy.rrook_moved[1];
    return *this;
}

#endif