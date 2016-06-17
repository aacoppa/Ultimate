#ifndef EVALUATION12341234
#define EVALUATION12341234

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "debug.h"

#define MAX 100000000.0
#define MIN -100000000.0

/* Converts 0-4 winning pattern where 0 is impossible to win, 1 is 0-in-the-pattern, 3 is 2-in-a-row, 4 is a won box
 */
static float LIKELYHOOD_OF_WINNING(int num) {
    if(num == 0) {
        return 0.0;
    } else if(num == 1) {
        return 4.5;
    } else if(num == 2) {
        return 12.5;
    } else if(num == 3){
        return 50.0;
    }
    return 100.0; //Won box :/
}
/* Returns the other turn
 */
int otherTurn(int turn);

/* Returns a float evaluation of the position
 * but generates a brand new list of
 * boxWorths
 */
float evaluatePositionRaw(uint32_t ** board, int turn);
/* Returns a float evaluation of the position
 * given an existing list of box worths
 */
float evaluatePosition(uint32_t ** board, int turn, float * boxWorths);

/* Given the chance of winning each box
 * we calculate the total chance of winning
 * by looking at each pattern and multiplying
 * the scores of each box in the pattern
 */
float overallLikelyhoodOfWinning(uint32_t ** board, int turn, float * boxWorths);

/* Calculates the likehood of winning a box by looking
 * at each pattern inside of it, calling
 * winningLikelyhoodOfPattern() and combining the
 * heuristic approximations of each pattern's worth
 * together
 */
float likelyhoodOfWinningBox(uint32_t ** board, int box, int turn);

/* Returns 0-4 likelyhood of winning a box with
 * a given pattern, 0 is impossible, 4 is won for a specific player
 */
int winningLikelyhoodOfPattern(uint32_t ** board, int box, int * squares, int turn);


#endif
