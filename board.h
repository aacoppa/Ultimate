#ifndef BOARD192381
#define BOARD192381

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define EX 1
#define OH 2


#define FREEBIE 4
#define WON 8

/* Generate 9 3x3 boxes to represent the board
 * inside each boxes there exists a uint32_t representation
 * of what the box contains, 9 squares of X, O, or null
 */
void * initializeBoard();

/* Frees the boards memory
 */
void deleteBoard(uint32_t ** board);

/* Sets a given square to EX, OH or null
 *
 *  A square is represented by 2-bits of information
 *  square == 0x0 -> square is empty
 *  square == 0x1 -> square is EX
 *  square == 0x2 -> square is OH
 *  square == 0x3 -> undefined
 *
 *  uint32_t representation is
 *  2-bits of box state + 12 bits scrap
 *  + 9 x 2-bits of squares
 *  e.g.
 *  most signifigant -> least signifigant
 *  box is uncaptued... scrap bits...9 squares
 *  00 ...              12 * 0       9x2-bits
 *  For 32 total bits
 */
void setSquare(void * _board, int _box, int _square, int _value);

/* Returns a squares value
 */
int getSquare(void * _board, int _box, int _square);

/* Sets overall box state
 */
void setBoxState(void * board, int _box, int _value);

/* Returns overall box state
 */
int getBoxState(void * _board, int _box);

/* Checks if a given move can be played
 */
int validMove(void * _board, int _box, int _square);

/* Compares squares in the same box
 * returns the bitwise and of each square,
 * so 0 if not all the same, X if all X's,
 * O if all O's
 */
int equalSquares2(uint32_t * box, int sq1, int sq2);
int equalSquares3(uint32_t * box, int sq1, int sq2, int sq3);
int equalSquares(uint32_t * box, int * squares, int len);

/* List of all the ways a box can be won
 */
static int winningPatterns[8][3] = {
{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
{0, 4, 8}, {2, 4, 6}
};

/* Returns what the state of our box is
 */
int isBoxCaptured(void * board, int box);
int isBoardWon(void * board);

/* Places the square and returns information about whether the box and game have been won
 */
int makeMove(uint32_t ** board, int box, int square, int turn);

/* Un-does the result of a makeMove call
 * including reseting boxStates
 *
 */
void unmakeMove(uint32_t ** board, int box, int square, int val);
#endif
