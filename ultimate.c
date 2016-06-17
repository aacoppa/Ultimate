#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


#include "tests.h"
#include "board.h"
#include "game.h"



int main(int argc, char ** argv) {
    void * _board = initializeBoard();
#if DEBUG
    runAllTests();
    exit(0);
#endif
    play();
    
}
