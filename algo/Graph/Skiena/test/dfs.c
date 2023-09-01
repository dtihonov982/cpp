#include "graph.h"
#include <stdio.h>

int main() {
    graph g;
    initialize_graph(&g, false);
    read_graph(&g, false);
    printf("graph\n");
    print_graph(&g);
    printf("dfs\n");
    initialize_search(&g);
    dfs(&g, 1);
    free_graph(&g);
    return 0;
}
