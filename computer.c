#include "computer.h"

void initComputer(uint32_t ** board, int turn) {
    number_of_threads = 0;
    //return tree_moves[0];
}
int getComputerMove(uint32_t ** board, int box, int freebie, int turn) {
    boxWorths = calloc(9, sizeof(float)); 
    for(int j = 0; j < 9; j++) {
        boxWorths[j] = likelyhoodOfWinningBox(board, j, turn);
        
    }
    float value = minimax(board, box, freebie, turn, 0, MIN, MAX);
    free(boxWorths);
    return tree_moves[0];
}
//Alpha is our lower bound, beta is our upper bound
//so alpha <= score <= beta
float minimax(uint32_t ** board, int box, int freebie, int turn, int depth, float alpha, float beta) {
    if(depth == MAX_DEPTH)
        //Always evaluate from the position of our first player
        return evaluatePosition(board, (depth % 2) ? otherTurn(turn) : turn, boxWorths);
    if(depth % 2) {
        //return min(board, turn, box, freebie, depth + 1);
        return min(board, box, freebie, turn, depth, alpha, beta);
    } else {
        return max(board, box, freebie, turn, depth, alpha, beta);
    }
}
float max(uint32_t ** board, int box, int freebie, int turn, int depth, float alpha, float beta) {
    /* Value 
     */
    queue_t * possibleMoves = generatePossibleMoves(board, box, freebie);
    //pthread_mutex_lock(&thread_count_m);
    int move = 0;
    int best_move = -1;
    //float max = MIN;
    while((move = dequeue(possibleMoves)) != -1) {
        if(best_move == -1)
            best_move = move;
        if(freebie)
            box = move / 10;
        int square = move % 10;

        //Store the old worth of our box
        //
        float old_worth = boxWorths[box];

        int val = makeMove(board, box, square, turn);
        boxWorths[box] = likelyhoodOfWinningBox(board, box, turn);
        
        //Special case if the game is over return the max value
        if(val & WON) {
            tree_moves[depth] = move;
            deleteQueue(possibleMoves);
            unmakeMove(board, box, square, val);
            return MAX;
        }
        
        //Recurse down the tree
        int nextBox = box;
        if(!isBoxCaptured(board, square)) {
            nextBox = square;
        }
        float branch_value = minimax(board, nextBox, val, otherTurn(turn), depth + 1, alpha, beta);
        unmakeMove(board, box, square, val);

        //Reset the box rating
        boxWorths[box] = old_worth;

        //Update our best value
        if(branch_value > alpha) {
            best_move = move;
            alpha = branch_value;
        }
        
        //Attempt to prune
        if(alpha >= beta) {
            deleteQueue(possibleMoves);
            return alpha;
        }
    }
    tree_moves[depth] = best_move;
    deleteQueue(possibleMoves);
    return alpha;
    //pthread_mutex_unlock(&thread_count_m);
}

float min(uint32_t ** board, int box, int freebie, int turn, int depth, float alpha, float beta) {
    queue_t * possibleMoves = generatePossibleMoves(board, box, freebie);
    //pthread_mutex_lock(&thread_count_m);
    int move = 0;
    int best_move = -1;
    while((move = dequeue(possibleMoves)) != -1) {
        if(best_move == -1)
            best_move = move;
        if(freebie)
            box = move / 10;
        int square = move % 10;

        float old_worth = boxWorths[box];
        int val = makeMove(board, box, square, turn);
        //Calculate new worth of our box, use other turn since we are in min
        boxWorths[box] = likelyhoodOfWinningBox(board, box, otherTurn(turn));

        //Special case if the game is over return the max value
        if(val & WON) {
            tree_moves[depth] = move;
            deleteQueue(possibleMoves);
            unmakeMove(board, box, square, val);
            return MIN;
        }
        
        int nextBox = box;
        if(!isBoxCaptured(board, square)) {
            nextBox = square;
        }
        //Recurse down the tree
        float branch_value = minimax(board, nextBox, val, otherTurn(turn), depth + 1, alpha, beta);
        //Set our max value to be the best move we find
        unmakeMove(board, box, square, val);

        //Reset the box rating
        boxWorths[box] = old_worth;

        if(branch_value < beta) {
            best_move = move;
            beta = branch_value;
        }
        if(beta <= alpha) {
            deleteQueue(possibleMoves);
            return beta;
        }
    }
    tree_moves[depth] = best_move;
    deleteQueue(possibleMoves);
    return beta;
    //pthread_mutex_unlock(&thread_count_m);
}

queue_t * generatePossibleMoves(uint32_t ** board, int box, int freebie) {
    queue_t * possibleMoves = initQueue();
    int square = 0;
    if(freebie) {
        for(box = 0; box < 9; box++) {
            for(square = 0; square < 9; square++) {
                if(validMove(board, box, square))
                    enqueue(possibleMoves, box, square);
            }
        }
    } else {
        for(square = 0; square < 9; square++) {
            if(validMove(board, box, square))
                enqueue(possibleMoves, box, square);
        }
    }
    return possibleMoves;
}

