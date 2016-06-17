#include "queue.h"

void deleteQueue(queue_t * queue) {
    while(queue->head) {
        node * temp = queue->head;
        free(queue->head);
        queue->head = temp->next;
    }
    free(queue);
}
queue_t * initQueue() {
    queue_t * queue = malloc(sizeof(queue_t));
    queue->head = malloc(sizeof(node));
    queue->tail = queue->head;
    queue->head->next = NULL;
    return queue;
}

void enqueue(queue_t * queue, int box, int square) {
    queue->tail->boxSquare = 10 * box + square;
    queue->tail->next= malloc(sizeof(node));
    queue->tail = queue->tail->next;

    queue->tail->next = NULL;
}

int dequeue(queue_t * queue) {
    if(queue->head == queue->tail)
        return -1;
    node * temp = queue->head;
    queue->head = queue->head->next;

    int retVal = temp->boxSquare;
    free(temp);
    return retVal;
}

