#include <stdio.h>
#include "queue.h"

int main() {
    int i;
    queue q;

    init_queue(&q);
    printf("empty: %s\n", empty_queue(&q) ? "true" : "false");
    enqueue(&q, 101);
    enqueue(&q, 202);
    enqueue(&q, 303);
    printf("empty: %s\n", empty_queue(&q) ? "true" : "false");
    for (i = 0; i < 4; i++) {
        printf("element %d is %d\n", i, dequeue(&q));
        printf("empty: %s\n", empty_queue(&q) ? "true" : "false");
    }
    enqueue(&q, 404);
    enqueue(&q, 505);
    for (i = 0; i < 3; i++) {
        printf("element %d is %d\n", i, dequeue(&q));
        printf("empty: %s\n", empty_queue(&q) ? "true" : "false");
    }
    return 0;
}
