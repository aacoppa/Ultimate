#ifndef TESTS0101924
#define TESTS0101924

//Define before all other includes...
#define DEBUG 0

#include "board.h"
#include "queue.h"
#include "computer.h"
#include <math.h>
#include <time.h>


//Runs every test
void runAllTests();

//Tests setting and getting squares
int testSettingSquares();


//Tests equal functions and isBoxCaptured
int testBoxCapturing();
//Tests the queue's functionality
//Doesn't work when adding after a final dequeue
int testQueue();

//Tests the make move function
//and whether checking for wins
//and full squares works
//
int testMakeMove();

//Tests the evaluation pattern counting
//and win finding functions
//
int testCountingPatterns();

//Tests functions in computer.c
//
int testComputer();
#endif
