#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct item item;

typedef struct {
    item *first;
    item *last;
} queue;

struct item {
    int value;
    item *next;
};

void init_queue(queue *q);
bool empty_queue(const queue *q);
void enqueue(queue *q, int value);
int dequeue(queue *q);

#endif //QUEUE_H
