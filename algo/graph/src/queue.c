#include "queue.h"
#include <stdlib.h>


void init_queue(queue *q) {
    q->first = NULL;
    q->last = NULL;
}

bool empty_queue(const queue *q) {
    return q->first == NULL;
}

void enqueue(queue *q, int value) {
    item *x;
    x = malloc(sizeof(item));
    x->value = value;
    x->next = NULL;
    if (!empty_queue(q)) {
        q->last->next = x;
        q->last = x;
    }
    else {
        q->first = q->last = x;
    }
}

int dequeue(queue *q) {
    if (!empty_queue(q)) {
        item *first = q->first;
        int result = first->value;
        q->first = first->next;
        free(first);
        return result;
    }
    else {
        return 1;
    }
}
