#ifndef __BOARD_H__
#define __BOARD_H__

class Board{
    char sides[8][8];
    char pieces[8][8];
public:
    Board();
    Board(char(*)[8],char(*)[8]);
    friend class DisplayedBoard;
};

#endif