#ifndef COMPUTER123413495
#define COMPUTER123413495

#include "board.h"
#include "queue.h"
#include "evaluation.h"
#include <pthread.h>
#include <semaphore.h>
#include "tests.h"

#define MAX_THREADS 8
#define MAX_DEPTH 6



int tree_moves[MAX_DEPTH];
int number_of_threads;

/* Initilizes the computer, sets up threading, etc
 */
void initComputer(uint32_t ** board, int turn);

/* Outward facing call that will return the 
 * computers optimal move
 */
int getComputerMove(uint32_t ** board, int box, int freebie, int turn);

/* Returns queue of all possible moves that can be
 * made in a turn
 */
queue_t * generatePossibleMoves(uint32_t ** board, int box, int freebie);


/* Implements the minimax algorithm
 * calling max and min respectively
 */
float minimax(uint32_t ** board, int box, int freebie, int turn, int depth);

/* Does the max call of the minimax algorithm, returns best possible move sequence and score of it
 */
float max(uint32_t ** board, int box, int freebie, int turn, int depth);

/* Does the min call of the minimax algorithm, returns worst possible move sequence and score of it
 */
float min(uint32_t ** board, int box, int freebie, int turn, int depth);

#endif
