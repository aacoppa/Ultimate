var width, height;
int [] board;
int [] boardStates;
int backColor = 255;
int highlightFill = 150;
int edgeColor = 100;
var disconnect = false;
int ourTurn = 1;
int theirTurn = 2;
int turn;
int game_index;
int ended;
int currentBox;
int FREEBIE = 11;
int LOST = 12;
int WON = 13;
int TIE = 14;
int TIMEOUT = 15;
int animY = 0;
void setup()
{
    ended = 0;
    width = 400;
    height = 400;
    game_index = setupGame();
    turn = ourTurn;
    freebie = 0;
    currentBox = 0;

    board = new int[100];
    for(int i = 0; i < 100; i++) {
        board[i] = 0;
    }
    boardStates = new int[10];
    for(int i = 0; i < 9; i++) {
        boardStates[i] = 0;
    }
    /*
       board[8] = 1;
       board[11] = 1;
       board[51] = 2;
       boardStates[2] = 1;
       boardStates[6] = 1;
       boardStates[7] = 2;
       */
    size(width,height);
    background(255);
    fill(255);
    //noLoop();
    PFont fontA = loadFont("courier");
    textFont(fontA, 14);  
}
void drawBoardStates(int a, int b) {
    bnum = a + 3 * b;
    fill(backColor);
    stroke(edgeColor);
    if(boardStates[bnum] != 0) {
        int x = (width+2) * a/3;
        int y = (height+2) *b/3;
        rect(x, y, width/3 - 2, height/3-2);

        x += width / 6;
        y += height / 4-10;
        textAlign(CENTER);
        fill(255, 0, 0);
        textSize(96);
        if(boardStates[bnum] == 1) {
            text("X", x, y);
        } else {
            text("O", x, y);
        }
    }

}
void drawLetter(int a, int b) {
    int x = a * width / 9 + width / 18;
    int y = b * height / 9 + height / 12;
    fill(255, 0, 0);
    textSize(32);
    textAlign(CENTER);

    if(board[9*b+a] == 1) {
        text("X", x, y);
    }
    else if(board[9*b+a] == 2) {
        text("O", x, y);
    }
}

void drawBoard() {
    for(int i = 1; i < 9; i++) {
        stroke(edgeColor);
        strokeWeight(1);
        if(i % 3 == 0) {
            strokeWeight(5);
        }
        line(width * i / 9, 0, width * i / 9, height);
    }
    for(int i = 1; i < 9; i++) {
        strokeWeight(1);
        if(i % 3 == 0) {
            strokeWeight(5);
        }
        line(0, height* i / 9, width, i*height/9);
    }

    for(int b = 0; b < 9; b++) {
        for(int sq = 0; sq < 9; sq++) {
            drawLetter(sq, b);
        }
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            drawBoardStates(i, j);
        }
    }

}
void setupGame() {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "http://localhost:8888/tictactoe/setup", false ); // false for synchronous request
    xmlHttp.send( null );
    var text = xmlHttp.responseText;
    return parseInt(text);
}
void makeTheirMove(var s) {
    var res = s.split(":");
    int v = parseInt(res[1]);
    if(v == LOST) {
        ended = theirTurn;
    } else if(v == WON) {
        ended = ourTurn;
    } else if(v == TIE) {
        ended = TIE;
    } else if(v == FREEBIE) {
        freebie = 1;
    } else if(v == TIMEOUT) {
        disconnect = true;
    } else {
        freebie = 0;
        currentBox = parseInt(res[1]);
    }
    boardStates[parseInt(res[2])] = ourTurn;
    boardStates[parseInt(res[3])] = theirTurn;
    //println(res[0] + " " res[1]);
    board[parseInt(res[0])] = theirTurn;
    turn = ourTurn;
}
int sendMove(var move) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "http://localhost:8888/tictactoe/"+game_index+"/"+move, true); // false for synchronous request
    xmlHttp.onreadystatechange = function() {
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
            makeTheirMove(xmlHttp.responseText);;
        }
    };
    xmlHttp.send();
}

void checkForMove() {
    if(mousePressed) {
        int boardNumberX = (int) ((mouseX * 3) / width);
        int boardNumberY = (int) ((mouseY * 3) / height);
        var smallMouseX = mouseX - boardNumberX * width / 3;
        var smallMouseY = mouseY - boardNumberY * height/ 3;
        int squareX = (int) ((smallMouseX) / (width / 9));
        int squareY = (int) ((smallMouseY) / (width / 9));
        //println("mouseX, boardNumberX, width: " + mouseX + ", " + boardNumberX + " " + width);
        int b = boardNumberX + 3 * boardNumberY;
        int sq = squareX + 3 * squareY;

        int otherSquareNum = 9 * (3*boardNumberY + squareY) + 3*boardNumberX + squareX;
        if(freebie == 1 || b == currentBox) {
            if(boardStates[b] == 0 && turn == ourTurn
                    && board[otherSquareNum] == 0) {
                board[otherSquareNum] = ourTurn;
                println("Sending move: " + 10*b+sq);
                sendMove(10 * b + sq);
                turn = theirTurn;
            }
        }
    }
}
void highlightBox(int b) {
    int x = b % 3;
    int y = (int) (b / 3);

    fill(highlightFill);
    rect(width * x / 3, height * y / 3, width / 3, height / 3);

}
void draw(){  
    background(backColor);
    if(freebie) {
        background(highlightFill);
    } else {
        highlightBox(currentBox);
    }
    drawBoard();
    if(ended == 0 && disconnect == false) {
        checkForMove();
    } else {
        textAlign(CENTER);
        fill(0);
        if(ended == 2) {
            text("Victory", width / 2, animY);
        } else if(ended == 1) {
            text("Defeat", width / 2, animY);
        } else if(ended == TIE) {
            text("Tie", width / 2, animY);
        } else if(disconnect == true) {
            text("Game timed out :(", width/2, animY);
        }
        animY += 1;
        if(animY > height) {
            location.reload();
        }
    }
    if(game_index == -1) {
        textAlign(CENTER);
        background(0);
        fill(255);
        textSize(12);
        text("Too many players connected right now, try again soon", width/2, height/2);
    }
    if(game_index == -2) {
        textAlign(CENTER);
        background(0);
        fill(255);
        textSize(12);
        text("The AI server isn't running right now...", width/2, height/2);
    }
}
