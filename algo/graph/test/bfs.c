#include "graph.h"
#include <stdio.h>

int main() {
    graph g;
    initialize_graph(&g, false);
    read_graph(&g, false);
    print_graph(&g);
    printf("----------\n");
    bfs(&g, 1);
    free_graph(&g);
    return 0;
}
