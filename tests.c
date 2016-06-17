#include "tests.h"

void runAllTests() {
    printf("DEBUG: Running tests...\n\n");
    printf("====================\n");
    printf("board.c\n");
    printf("====================\n");
    testSettingSquares();
    testBoxCapturing();
    testMakeMove();
    printf("====================\n");
    printf("queue.c\n");
    printf("====================\n");
    testQueue();
    printf("====================\n");
    printf("evaluate.c\n");
    printf("====================\n");
    testCountingPatterns();
    printf("====================\n");
    printf("computer.c\n");
    printf("====================\n");
    testComputer();
}

int testSettingSquares() {
    printf("Testing initializeBoard()...\n");
    void * _board = initializeBoard();
    printf("initializeBoard() works!\n");
    printf("Testing setSquare() and getSquare()...\n");

    setSquare(_board, 1, 1, EX);
    assert(getSquare(_board, 1, 1) == EX);

    setSquare(_board, 3, 2, OH);
    assert(getSquare(_board, 1, 1) == EX);
    assert(getSquare(_board, 3, 2) == OH);

    setSquare(_board, 1, 8, OH);
    assert(getSquare(_board, 1, 1) == EX);
    assert(getSquare(_board, 3, 2) == OH);
    assert(getSquare(_board, 1, 8) == OH);

    setSquare(_board, 1, 0, 0);
    assert(getSquare(_board, 1, 1) == EX);
    assert(getSquare(_board, 3, 2) == OH);
    assert(getSquare(_board, 1, 8) == OH);
    assert(getSquare(_board, 1, 0) == 0);

    setSquare(_board, 1, 1, 0);
    assert(getSquare(_board, 1, 1) == 0);
    printf("setSquare() and getSquare() work!\n");


    printf("Testing setBoxState() and getBoxState()...\n");
    setSquare(_board, 4, 0, EX);
    setSquare(_board, 4, 1, OH);
    setBoxState(_board, 4, OH);
    assert(getBoxState(_board, 4) == OH);

    setBoxState(_board, 8, EX);
    assert(getBoxState(_board, 8) == EX);

    setBoxState(_board, 8, 0);
    assert(getBoxState(_board, 8) == 0);
    assert(getSquare(_board, 4, 0) == EX);
    assert(getSquare(_board, 4, 1) == OH);
    printf("setBoxState() and getBoxState() work!\n");

    printf("Testing deleteBoard()...\n");
    deleteBoard(_board);
    printf("deleteBoard() works!\n");
    return 0;
}

int testBoxCapturing() {
    uint32_t ** _board = (uint32_t **) initializeBoard();
    
    setSquare(_board, 1, 0, OH);
    printf("Testing equalSquaresN() functions...\n");
    setSquare(_board, 1, 1, OH);
    setSquare(_board, 1, 2, OH);
    assert(equalSquares2(_board[1], 0, 1));
    assert(equalSquares3(_board[1], 0, 1, 2));

    int * arr = malloc(3 * sizeof(int));
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    assert(equalSquares(_board[1], arr, 3));

    setSquare(_board, 1, 1, EX);
    setSquare(_board, 1, 4, EX);
    assert(equalSquares2(_board[1], 1, 4));

    setSquare(_board, 2, 5, 0);
    setSquare(_board, 2, 8, 0);

    assert(!equalSquares2(_board[1], 1, 2));

    printf("equalSquaresN() functions work!\n");
    printf("Testing isBoxCaptured()...\n");

    setSquare(_board, 3, 0, EX);
    setSquare(_board, 3, 3, EX);
    setSquare(_board, 3, 6, EX);
    assert(isBoxCaptured(_board, 3) == EX);
    
    setSquare(_board, 3, 0, OH);
    assert(!isBoxCaptured(_board, 3));

    setSquare(_board, 3, 1, 0);
    setSquare(_board, 3, 4, 0);
    setSquare(_board, 3, 7, 0);
    assert(!isBoxCaptured(_board, 3));

    setSquare(_board, 4, 0, OH);
    setSquare(_board, 4, 4, OH);
    setSquare(_board, 4, 8, OH);
    assert(isBoxCaptured(_board, 4));
    printf("isBoxCaptured() works!\n");

    deleteBoard(_board);

    printf("Testing isBoardWon()...\n");
    for(int i = 0; i < 8; i++) {

        _board = initializeBoard();
        for(int j = 0; j < 3; j++) {
            assert(!isBoardWon(_board));
            setBoxState(_board, winningPatterns[i][j], EX);
            
        }
        assert(isBoardWon(_board) & WON);
        deleteBoard(_board);
    }
    printf("isBoardWon() works!\n");
    return 0;
}

int testQueue() {
    printf("Testing queue out...\n");
    queue_t * test = initQueue();
    printf("initQueue() works!\n");
    enqueue(test, 1, 1);
    enqueue(test, 2, 1);
    enqueue(test, 1, 2);
    enqueue(test, 2, 2);

    printf("enqueue() works!\n");

    assert(dequeue(test) == 11);
    assert(dequeue(test) == 21);
    enqueue(test, 4, 1);
    assert(dequeue(test) == 12);
    assert(dequeue(test) == 22);
    assert(dequeue(test) == 41);
    assert(dequeue(test) == -1);
    printf("dequeue() works!\n");
    deleteQueue(test);
    
    printf("deleteQueue() works!\n");

    return 0;
}

int testMakeMove() {
    uint32_t ** board = initializeBoard();
    
    printf("Testing validMove()...\n");
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
        assert(validMove(board, i, j));
        }
    }
    setSquare(board, 6, 7, EX);
    setSquare(board, 4, 6, OH);

    setBoxState(board, 3, OH);
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(i == 3 || (i == 4 && j == 6) ||
              (i == 6 && j == 7)) {

            } else {
                assert(validMove(board, i, j));
            }
        }
    }
    printf("validMove() works!\n");

    deleteBoard(board);
    board = initializeBoard();

    printf("Testing makeMove() and unmakeMove()...\n");
    assert((FREEBIE & WON) == 0);
    assert(makeMove(board, 0, 0, EX) == 0);
    assert(makeMove(board, 0, 1, EX) == 0);
    
    assert(makeMove(board, 0, 2, EX) & FREEBIE);
    
    unmakeMove(board, 0, 2, FREEBIE);
    assert(getSquare(board, 0 , 2) == 0);
    assert(getBoxState(board, 0) == 0);

    assert(makeMove(board, 0, 2, EX) & FREEBIE);

    assert(makeMove(board, 1, 0, 0) == 0);
    assert(makeMove(board, 1, 3, OH) == 0);
    assert(makeMove(board, 1, 4, OH) == 0);
    assert(makeMove(board, 1, 5, OH) & FREEBIE);

    assert(makeMove(board, 3, 0, EX) == 0);
    assert(makeMove(board, 3, 1, OH) == 0);
    assert(makeMove(board, 3, 2, EX) == 0);
    assert(makeMove(board, 3, 4, EX) == 0);
    assert(makeMove(board, 3, 8, EX) & FREEBIE);

    assert(makeMove(board, 6, 2, EX) == 0);
    assert(makeMove(board, 6, 6, EX) == 0);
    assert(makeMove(board, 6, 4, EX) & WON);
    unmakeMove(board, 6, 4, WON);
    assert(getSquare(board, 6, 4) == 0);
    assert(getBoxState(board, 6) == 0);
    printf("makeMove() and unmakeMove() work!\n");

    deleteBoard(board);
    return 0;
}

int testCountingPatterns() {
    uint32_t ** board = initializeBoard();
    printf("Testing winningLikelyhoodOfPattern()...\n");
    int * pattern = calloc(3, sizeof(int));
    pattern[0] = 0;
    pattern[1] = 1;
    pattern[2] = 2;
    assert(winningLikelyhoodOfPattern(board, 0, pattern, EX) == 1);
    assert(winningLikelyhoodOfPattern(board, 0, pattern, OH) == 1);
                                                                 
    setSquare(board, 0, 0, EX);                                  
    assert(winningLikelyhoodOfPattern(board, 0, pattern, EX) == 2);
    assert(winningLikelyhoodOfPattern(board, 0, pattern, OH) == 0);
                                                                           
    setSquare(board, 0, 2, EX);                                            
    assert(winningLikelyhoodOfPattern(board, 0, pattern, EX) == 3);

    setSquare(board, 0, 1, OH);
    assert(winningLikelyhoodOfPattern(board, 0, pattern, EX) == 0);
    assert(winningLikelyhoodOfPattern(board, 0, pattern, OH) == 0);

    deleteBoard(board);

    board = initializeBoard();
    printf("winningLikelyhoodOfPattern() works!\n");
    printf("Testing likelyhoodOfWinningBox()...\n");


    float v1 = likelyhoodOfWinningBox(board, 0, EX);
    assert(v1 == LIKELYHOOD_OF_WINNING(1));

    float v2 = likelyhoodOfWinningBox(board, 0, OH);
    assert(v2 == LIKELYHOOD_OF_WINNING(1));

    setSquare(board, 0, 1, EX);

    v2 = likelyhoodOfWinningBox(board, 0, OH);
    assert(v2 == LIKELYHOOD_OF_WINNING(1) * 6 / 8);

    v1 = likelyhoodOfWinningBox(board, 0, EX);
    assert(v1 == (LIKELYHOOD_OF_WINNING(1)* 6 + LIKELYHOOD_OF_WINNING(2) * 2)/8);

    setSquare(board, 0, 0, EX);
    v1 = likelyhoodOfWinningBox(board, 0, OH);
    assert(v1 == (LIKELYHOOD_OF_WINNING(1) * 4 / 8));
    
    v2 = likelyhoodOfWinningBox(board, 0, EX);
    assert(v2 == (LIKELYHOOD_OF_WINNING(1) * 4 + LIKELYHOOD_OF_WINNING(2) * 3 + LIKELYHOOD_OF_WINNING(3) * 1)/8);

    deleteBoard(board);

    printf("likelyhoodOfWinningBox() works!\n");
    printf("Testing evaluatePosition() and overallLikelyhoodOfWinning()...\n");
    board = initializeBoard();
    float score = evaluatePosition(board, EX);
    assert(score == 8 * pow(LIKELYHOOD_OF_WINNING(1), 3));

    setSquare(board, 1, 1, EX);
    v1 = evaluatePosition(board, EX);
    v2 = evaluatePosition(board, OH);

    assert(v1 > v2);


    assert(v1 == 2 * (likelyhoodOfWinningBox(board, 1, EX) * likelyhoodOfWinningBox(board, 4, EX) * likelyhoodOfWinningBox(board, 7, EX)) + 6 * pow(LIKELYHOOD_OF_WINNING(1), 3));


    printf("evaluatePosition() and overallLikelyhoodOfWinning() work!\n");
    deleteBoard(board);


    return 0;
}

int testComputer() {
    uint32_t ** board = initializeBoard();
    printf("Testing generatePossibleMoves()...\n");
    queue_t * queue = generatePossibleMoves(board, 3, 0);

    int move = 0;
    int expectedMove = 30;
    while((move = dequeue(queue)) != -1) {
        assert(expectedMove == move);
        expectedMove++;
    }

    deleteQueue(queue);
    queue = generatePossibleMoves(board, 3, 1);

    move = -1;
    expectedMove = 0;
    while((move = dequeue(queue)) != -1) {
        assert(expectedMove == move);
        expectedMove++;
        if(expectedMove % 10 == 9)
            expectedMove++;
    }

    setSquare(board, 5, 6, EX);
    setSquare(board, 4, 7, OH);

    move = -1;
    expectedMove = 0;
    while((move = dequeue(queue)) != -1) {
        if(expectedMove != 56 && expectedMove != 47) {
            assert(expectedMove == move);
        }
        expectedMove++;
        if(expectedMove % 10 == 9)
            expectedMove++;
    }

    setSquare(board, 5, 4, EX);
    setSquare(board, 5, 2, EX);

    setSquare(board, 4, 6, OH);
    setSquare(board, 4, 8, OH);
    while((move = dequeue(queue)) != -1) {
        if(expectedMove < 40 && expectedMove > 59) {
            assert(expectedMove == move);
        }
        expectedMove++;
        if(expectedMove % 10 == 9)
            expectedMove++;
    }


    deleteQueue(queue);
    deleteBoard(board);
    printf("generatePossibleMoves() works!\n");
    
    board = initializeBoard();
    printf("testing minimax()...\n");

    assert(minimax(board, 0, 0, EX, MAX_DEPTH, MIN, MAX) ==
           evaluatePosition(board, EX));
    //Will only call min!
    assert(minimax(board, 0, 0, EX, MAX_DEPTH-1, MIN, MAX) == 
           min(board, 0, 0, EX, MAX_DEPTH-1, MIN, MAX));

    deleteBoard(board);

    for(int i = 0; i < 8; i++) {
        board = initializeBoard();
        setBoxState(board, winningPatterns[i][0], EX);
        setBoxState(board, winningPatterns[i][1], EX);

        setSquare(board, winningPatterns[i][2], 0, EX); 
        setSquare(board, winningPatterns[i][2], 4, EX); 

        assert((MAX_DEPTH - 1) % 2);

        assert(minimax(board, winningPatterns[i][2], 0, EX, MAX_DEPTH - 1, MIN, MAX) == MIN);
        assert(minimax(board, 0, 1, EX, MAX_DEPTH - 1, MIN, MAX) == MIN);
        assert(tree_moves[MAX_DEPTH-1] == winningPatterns[i][2] * 10 + 8);
        deleteBoard(board);
    }

    board = initializeBoard();
    setBoxState(board, 1, OH);
    setBoxState(board, 4, OH);

    setSquare(board, 0, 0, EX);
    setSquare(board, 0, 1, EX);
    setSquare(board, 0, 3, EX);
    setSquare(board, 0, 4, EX);

    setSquare(board, 7, 2, OH);
    setSquare(board, 7, 6, OH);

    assert(minimax(board, 0, 0, EX, MAX_DEPTH - 2, MIN, MAX) == MIN);
    assert(tree_moves[MAX_DEPTH-1] == 74);
    assert(tree_moves[MAX_DEPTH-2] == 2);
    deleteBoard(board);

    board = initializeBoard();
    setBoxState(board, 3, EX);
    setBoxState(board, 5, EX);
    setBoxState(board, 0, OH);

    setSquare(board, 4, 0, EX);
    setSquare(board, 4, 2, EX);
    setSquare(board, 4, 6, EX);
    setSquare(board, 4, 8, EX);
    setSquare(board, 4, 1, OH);
    setSquare(board, 4, 7, OH);


    setSquare(board, 7, 2, OH);
    setSquare(board, 7, 6, OH);

    assert(minimax(board, 1, 0, EX, MAX_DEPTH - 3, MIN, MAX) == MIN);
    deleteBoard(board);

    printf("minimax() sorta works!\n");


    board = initializeBoard();
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    getComputerMove(board, 0, 0, EX);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("time: %f\n", cpu_time_used);
    return 0;
}



