#include "board.h"

void * initializeBoard() {
    uint32_t ** board = malloc(9 * sizeof(uint32_t *));
    for(int i = 0; i < 9; i++) {
        board[i] = calloc(sizeof(uint32_t), 1);
    }
    return board;
}

/*  uint_32 representation exists as...
 *  9 sequences of two bits
 *  2 bit header representation of box + 12 bits scrap
 *  + 9 x 2bit representation of square 
 *  e.g.
 *  0x0 box hasnt been captured, 12 bits of scrap, 
 *  8th-square (bottom right), 7th-square (middle bottom) 
 *  ... 0th-square(top left)
 *  square == 0x0 -> square is empty
 *  square == 0x1 -> square is EX
 *  square == 0x2 -> square is OH
 *  square == 0x3 -> undefined
 */
void setSquare(void * _board, int _box, int _square, int _value) {
    assert(_box >= 0 && _box <= 8);
    assert(_square == 15 || (_square >= 0 && _square <= 8));
    assert(_value >= 0 && _value <= 2);
    uint32_t ** board = (uint32_t **) _board;
    uint32_t * box = board[_box];

    //Set the two bits of the square we're setting to 0
    uint32_t temp = 0xffffffff ^ (0x3 << (_square * 2));
    //Clear those two bits in our original box
    *box = *box & temp;
    //Set temp to contain only the two bits of value
    temp = 0x0 | (_value << (_square * 2));
    //Or temp with the original box
    *box = *box | temp;   
}

int getSquare(void * _board, int _box, int _square) {
    assert(_box >= 0 && _box <= 8);
    assert(_square == 15 || (_square >= 0 && _square <=8));
    uint32_t ** board = (uint32_t **) _board;
    uint32_t * box = board[_box];

    //Set only the two bits of the square we're getting
    //then clear every other bit in our box
    uint32_t temp = *box & (0x3 << (_square * 2));
    return temp >> (_square * 2);
}

void setBoxState(void * _board, int _box, int _value) {
    setSquare(_board, _box, 15, _value);
}

int getBoxState(void * _board, int _box) {
    return getSquare(_board, _box, 15);
}

int validMove(void * _board, int _box, int _square) {
    assert(_box >= 0 && _box <= 8);
    assert(_square >= 0 && _square <= 8);
    if(getBoxState(_board, _box)) 
        return 0; //Box has been filled
    if(getSquare(_board, _box, _square))
        return 0; //Square is occupied
    return 1;
}

int isBoxCaptured(void * _board, int _box) {
    uint32_t ** board = (uint32_t **) _board;
    uint32_t * box = board[_box];
    int ret = 0;
    for(int i = 0; i < 8; i++) {
        if((ret = equalSquares(box, winningPatterns[i], 3))) {
            return ret;
        }
    }
    return 0;
}
int equalSquares(uint32_t * box, int * squares, int len) {
    int _sqnum = squares[0];
    uint32_t t1 = (*box << (30-_sqnum*2)) >> 30;
    for(int i = 1; i < len; i++) {
        _sqnum = squares[i];
        t1 = t1 & ((*box << (30-_sqnum*2)) >> 30);
    }
    return t1;
}
int equalSquares2(uint32_t * box, int sq1, int sq2) {
    uint32_t t1 = (*box << (30-sq1*2)) >> 30;
    uint32_t t2 = (*box << (30-sq2*2)) >> 30;
    return t1 & t2;
}
int equalSquares3(uint32_t * box, int sq1, int sq2, int sq3) {
    uint32_t t1 = (*box << (30-sq1*2)) >> 30;
    uint32_t t2 = (*box << (30-sq2*2)) >> 30;
    uint32_t t3 = (*box << (30-sq3*2)) >> 30;
    return t1 & t2 & t3;
}

void unmakeMove(uint32_t ** board, int box, int square, int val) {
    setSquare(board, box, square, 0);
    if(val & FREEBIE || val & WON) {
        setBoxState(board, box, 0);
    }
}

int makeMove(uint32_t ** board, int box, int square, int turn) {
    setSquare(board, box, square, turn);
    int color = isBoxCaptured(board, box);
    if(color) {
        setBoxState(board, box, color);
        if(isBoardWon(board)) {
            return WON;
        }
        return FREEBIE;
    }
    return 0;
}


int isBoardWon(void * _board) {
    uint32_t ** board = (uint32_t **) _board;
    for(int i = 0; i < 8; i++) {
            if(getBoxState(board, winningPatterns[i][0]) & 
               getBoxState(board, winningPatterns[i][1]) & 
               getBoxState(board, winningPatterns[i][2]))
                return WON;
    }
    return 0;
}


void deleteBoard(uint32_t ** board) {
    for(int i = 0; i < 9; i++) {
        free(board[i]);
    }
    free(board);
}


