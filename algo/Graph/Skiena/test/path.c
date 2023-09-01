#include "graph.h"
#include <stdio.h>

int main() {
    graph g;
    int start, end;
    initialize_graph(&g, false);
    read_graph(&g, false);
    print_graph(&g);
    printf("Start and end: ");
    scanf("%d %d", &start, &end);
    printf("----------\n");
    initialize_search(&g);
    bfs(&g, 1);
    print_path(start, end);
    free_graph(&g);
    return 0;
}
