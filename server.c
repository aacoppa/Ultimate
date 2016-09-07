#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include "game.h"
#include <time.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <pthread.h>
#include "computer.h"


void* processClient(void* arg)  {
    int fd = *(int*)arg;
    printf("Accepted new client %d\n", fd);
    pthread_detach(pthread_self()); // no join() required
    uint32_t ** board = initializeBoard();
    computer * c = initComputer(10, 20, 60);
    struct timeval start, end;


    gettimeofday(&start, NULL);
    while( 1 ) {
        char buffer[1024];
        int recv = read(fd, buffer, 1023);
        if(recv <= 0) {
            break;
        }
        buffer[recv] = '\0';
        if(strcmp(buffer, "HEARTBEAT") == 0) {
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec);
            //printf("time taken: %f\n", time_taken);
            if(time_taken > 120) {
                break;
            }
            write(fd, "ALIVE\n", 6);
            continue;
        }
        gettimeofday(&start, NULL);
        int num = atoi(buffer);
        int box = num / 10;
        int square = num % 10;
        int val = makeMove(board, box, square, EX);
        int freebie = 0;
        char send[1024];
        int third = 10;
        int fourth = 10;
        if(val & WON) {
            strcpy(send, "-1:12");
            third = box;
        } else { 
            if(val & FREEBIE) {
                freebie = 1;
                third = box;
            }
            if(!isBoxCaptured(board, square)) {
                box = square;
            }

            num = getComputerMove(board, box, freebie, OH, c);
            box = num / 10;
            square = num % 10;
            val = makeMove(board, box, square, OH);

            ///Convert from our representation to theirs
            //
            int x = (box% 3) * 3 + square % 3;
            int y = (box/3) * 3 + (square/3);

            int theirMove = 9 * y + x;
            sprintf(send, "%d", theirMove);
            strcat(send, ":");
            if(val & FREEBIE) {
                strcat(send, "11");
                fourth = box;
            } else if(val & WON) {
                strcat(send, "13");
                fourth = box;
            } else {
                if(!isBoxCaptured(board, square)) {
                    box = square;
                }
                sprintf(send+strlen(send), "%d", box);
            }
        }
        strcat(send, ":");
        sprintf(send+strlen(send), "%d", third);
        strcat(send, ":");
        sprintf(send+strlen(send), "%d", fourth);
        int sent = write(fd, send, strlen(send)+1);
        if(sent != strlen(send)+1) {
            fprintf(stderr, "Didn't write full buffer to client!\n");
        }
        //displayBoard(board);
    }
    printf("Closing client %d\n", fd);
    close(fd);
    free(c);
    deleteBoard(board);
    return NULL;
}


void runServer() {

    char * port = "8001";
    struct addrinfo hints, *result;
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    char * o_val;
    o_val = (char * ) 1;
    struct timeval timeout;      
    timeout.tv_sec = 10;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &o_val, sizeof o_val);
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    int r;
    r = getaddrinfo(NULL, port, &hints, &result);
    if (r != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
            exit(1);
    }
    int b = bind(sock_fd, result->ai_addr, result->ai_addrlen);
    if(b == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    b = listen(sock_fd, 8);
    if(b == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    // Can now start accepting and processing client connections
    /*QUESTION 11*/
    int num = 0;
    while( 1 ) {
        int fd = accept(sock_fd, NULL, NULL);
        pthread_t new;
        pthread_create(&new, 0, &processClient, (void *) &fd);
    }
}
