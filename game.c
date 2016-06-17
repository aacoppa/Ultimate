#include "game.h"

int getPlayerMove(int freebie, int box) {
    int square = -1;
    while(1) {
        char * buff = NULL;
        size_t len = 0;
        if(freebie) {
            printf("Box %d!\n", box);
            printf("FREEBIE! Enter your move: (box number)(square number):\t");
        }
        else {
            printf("Box %d!\n", box);
            printf("Enter your move: (square number):\t");
        }
        getline(&buff, &len, stdin);
        if(freebie) {
            int box = (int)buff[0] - 48;
            if(box < 0 || box > 8) {
                square =-1;
                continue;
            }
            int square = (int)buff[1] - 48;
            if(square <= 0 || square >= 9) {
                square =-1;
                continue;
            }
            free(buff);
            return 10 * box + square;
        }
        int square = (int)buff[0] - 48;
        if(square < 0 || square > 8) {
            square =-1;
            continue;
        }
        free(buff);
        return square;
    }
    return -1;
}
int getMove(int freebie) {
    int move = rand() % 9;  
    if(freebie)
        move = move + 10 * (rand() % 9);
    return move;
}

void play() {
    srand(time(NULL));
    void * board = initializeBoard();

    int freebie = 0;
    int box = 0; //getBox();
    int turn = EX;
    while( 1 ) {
        //AUTO GENERATING MOVE CODE
        //
        //
        int square = 0;
        if(turn == EX) {
            square = getComputerMove(board, box, freebie, turn);
        } else {
            square = getPlayerMove(freebie, box);
        }
        if(square >= 10) {
            box = square / 10;
            square = square % 10;
            if(isBoxCaptured(board, box))
                continue;
        }
        //
        ////END OF TRASH
        if(!validMove(board, box, square))
            continue;
        //Clear the freebie
        //Place the piece
        int val = makeMove(board, box, square, turn);
        if(val & WON) {
            break;
        }
        else if(val & FREEBIE) {
            freebie = 1;
        }
        else {
            freebie = 0;
        }

        //Flip the turn
        if(turn == EX) {
            turn = OH;
        } else {
            turn = EX;
        }

        ////More trash display code
        //
        displayBoard(board);
        printf("Box: %d\t Square: %d\n", box, square);
        //
        ////End of display


        //If sent to won box, stay where you are
        if(!isBoxCaptured(board, square)) {
            box = square;
        }
    }
}

void displayBoard(void * board) {
    uint32_t ** _board = board;
    clear_screen();
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
    int height = 0;
    while(height < 9) {
        int x = 0;
        printf("||");
        while(x < 9) {
            int box = 3 * (height / 3) + x / 3;
            int square = x % 3 +  3 * (height % 3);
            int color = getSquare(board, box, square);
            if(color == EX) {
                printf(" X ");
            } else if(color == OH) {
                printf(" O ");   
            } else {
                printf("   ");
            }
            if(x % 3 == 2) {
                printf("||");
            } else {
                printf("|");
            }
            x++;
        }
        printf("\n");
        if(height %3 == 2) {
    printf("---------------------------------------------\n");
        }
    printf("---------------------------------------------\n");
        height++;
    }
}
void clear_screen()
{
    char buf[1024];
    char *str;

    tgetent(buf, getenv("TERM"));
    str = tgetstr("cl", NULL);
    fputs(str, stdout);
} 
