#ifndef QUEUE132134123
#define QUEUE132134123

#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
    int boxSquare;
    node * next;
}; 

typedef struct queue_t {
    node * head;
    node * tail;
} queue_t;

queue_t * initQueue();
void enqueue(queue_t * queue, int box, int square);
int dequeue(queue_t * queue);
void deleteQueue(queue_t * queue);

#endif
