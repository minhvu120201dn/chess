#ifndef __BOARD_H__
#define __BOARD_H__

class Board {
    char sides[8][8];
    char pieces[8][8];
    bool turn = WHITE;
    bool king_moved[2] = {false}, lrook_moved[2] = {false}, rrook_moved[2] = {false};
    char pawn_justmoved[2] = {-1};
    char king_row[2], king_col[2];
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
    Board(const Board&);
    inline Board& operator=(const Board&);
    inline bool ourKingSafe();
    inline std::pair<char,char> operator[](std::pair<char,char>);
    std::set<std::pair<char,char>>* availableMoves(char,char);
    void move(char,char,char,char);
};

inline Board& Board::operator=(const Board& copy) {
    for (char i = 0; i < 8; ++i)
        for (char j = 0; j < 8; ++j)
            this->sides[i][j] = copy.sides[i][j], this->pieces[i][j] = copy.sides[i][j];
    this->turn = copy.turn;
    this->king_moved[WHITE] = copy.king_moved[WHITE], this->king_moved[BLACK] = copy.king_moved[BLACK];
    this->lrook_moved[WHITE] = copy.lrook_moved[WHITE], this->lrook_moved[BLACK] = copy.lrook_moved[BLACK];
    this->rrook_moved[WHITE] = copy.rrook_moved[WHITE], this->rrook_moved[BLACK] = copy.rrook_moved[BLACK];
    this->king_row[WHITE] = copy.king_row[WHITE], this->king_row[BLACK] = copy.king_row[BLACK];
    this->king_col[WHITE] = copy.king_col[WHITE], this->king_col[BLACK] = copy.king_col[BLACK];
    return *this;
}

inline bool Board::ourKingSafe() {
    return this->checkKingSafe(this->king_col[this->turn],this->king_row[this->turn],this->turn);
}

inline std::pair<char,char> Board::operator[](std::pair<char,char> loc) {
    return std::make_pair(sides[loc.first][loc.second], pieces[loc.first][loc.second]);
    }

#endif