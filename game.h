#ifndef GAME1234132141
#define GAME1234132141
#include "board.h"
#include <termcap.h>
#include <time.h>
#include <stdlib.h>
#include "computer.h"
#include "debug.h"


void play();

void displayBoard(void * board);
void clear_screen();
int getMove(int freebie);
#endif
