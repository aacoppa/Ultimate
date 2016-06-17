#include "evaluation.h"

int otherTurn(int turn) {
    if(turn == EX) return OH;
    return EX;
}

float evaluatePosition(uint32_t ** board, int turn) {
    //Maintain a list of ways each board can be won
    //but skewed by a heuristic of likelyhood that
    //the box will be won, i.e. 2-in-a-row is worth
    //more than 1-in-a-row
    float * boxWorths = calloc(9, sizeof(float)); 
    for(int j = 0; j < 9; j++) {
        boxWorths[j] = likelyhoodOfWinningBox(board, j, turn);
        
    }
    return overallLikelyhoodOfWinning(board, turn, boxWorths);
}

float overallLikelyhoodOfWinning(uint32_t ** board, int turn, float * boxWorths) {
    //Check each big pattern and multiply the likelyhood of
    //winning each box together to get the likelyhood
    //of winning using that pattern
    float score = 0.0;
    for(int i = 0; i < 8; i++) {

        score += boxWorths[winningPatterns[i][0]] *
                 boxWorths[winningPatterns[i][1]] * 
                 boxWorths[winningPatterns[i][2]];
    }
    free(boxWorths);
    return score;
}
int winningLikelyhoodOfPattern(uint32_t ** board, int box, int * squares, int turn) {
    int v0 = getSquare(board, box, squares[0]);
    int v1 = getSquare(board, box, squares[1]);
    int v2 = getSquare(board, box, squares[2]);
    if((v0 | v1 | v2) & otherTurn(turn)) {
        return 0;
    }
    int num = 1;
    if(v0 & turn)
        num++;
    if(v1 & turn)
        num++;
    if(v2 & turn)
        num++;
    return num;
}
/* Return the likelyhood of a given box
 * being won
 */
float likelyhoodOfWinningBox(uint32_t ** board, int box, int turn) {
    int s = getBoxState(board, box);
    if(s == turn)
        return 100.0;
    else if(s == otherTurn(turn)) {
        return 0.0;
    }
    float boxWorth = 0.00;
    int total_combo_worth = 0;
    for(int i = 0; i < 8; i++) {
        //Pattern chance is 0-4 of our chance to
        //win with given pattern, 0 is impossible, 1
        //is all blank, 4 is we won it
        int pattern_chance = winningLikelyhoodOfPattern(
                              board, box, winningPatterns[i], 
                              turn);
        //Convert 0-4 score to probability of winning
        //0 through 100.0
        boxWorth += LIKELYHOOD_OF_WINNING(pattern_chance);
    }
    //Divide by total number of patterns
    return boxWorth / 8.0;
}
