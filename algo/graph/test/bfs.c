#include "graph.h"
#include <stdio.h>

int main() {
    graph g;
    initialize_graph(&g, false);
    read_graph(&g, false);
    printf("graph\n");
    print_graph(&g);
    printf("bfs\n");
    initialize_search(&g);
    bfs(&g, 1);
    printf("\nconnected components\n");
    connected_components(&g);
    free_graph(&g);
    return 0;
}
