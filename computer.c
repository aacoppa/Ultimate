#include "computer.h"

void initComputer(uint32_t ** board, int turn) {
    number_of_threads = 0;
    //return tree_moves[0];
}
int getComputerMove(uint32_t ** board, int box, int freebie, int turn) {
    float value = minimax(board, box, freebie, turn, 0);
    return tree_moves[0];
}
float minimax(uint32_t ** board, int box, int freebie, int turn, int depth) {
    if(depth == MAX_DEPTH)
        //Always evaluate from the position of our first player
        return evaluatePosition(board, (depth % 2) ? otherTurn(turn) : turn);
    if(depth % 2) {
        //return min(board, turn, box, freebie, depth + 1);
        return min(board, box, freebie, turn, depth);
    } else {
        return max(board, box, freebie, turn, depth);
    }
}
float max(uint32_t ** board, int box, int freebie, int turn, int depth) {
    queue_t * possibleMoves = generatePossibleMoves(board, box, freebie);
    //pthread_mutex_lock(&thread_count_m);
    int move = 0;
    int best_move = -1;
    float max = MIN;
    while((move = dequeue(possibleMoves)) != -1) {
        if(best_move == -1)
            best_move = move;
        if(freebie)
            box = move / 10;
        int square = move % 10;
        int val = makeMove(board, box, square, turn);

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
        float branch_value = minimax(board, nextBox, val, otherTurn(turn), depth + 1);
        //Set our max value to be the best move we find
        unmakeMove(board, box, square, val);
        if(branch_value > max) {
            best_move = move;
            max = branch_value;
        }
    }
    tree_moves[depth] = best_move;
    deleteQueue(possibleMoves);
    return max;
    //pthread_mutex_unlock(&thread_count_m);
}

float min(uint32_t ** board, int box, int freebie, int turn, int depth) {
    queue_t * possibleMoves = generatePossibleMoves(board, box, freebie);
    //pthread_mutex_lock(&thread_count_m);
    int move = 0;
    int best_move = -1;
    float min = MAX;
    while((move = dequeue(possibleMoves)) != -1) {
        if(best_move == -1)
            best_move = move;
        if(freebie)
            box = move / 10;
        int square = move % 10;
        int val = makeMove(board, box, square, turn);

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
        float branch_value = minimax(board, nextBox, val, otherTurn(turn), depth + 1);
        //Set our max value to be the best move we find
        unmakeMove(board, box, square, val);
        if(branch_value < min) {
            best_move = move;
            min = branch_value;
        }
    }
    tree_moves[depth] = best_move;
    deleteQueue(possibleMoves);
    return min;
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

