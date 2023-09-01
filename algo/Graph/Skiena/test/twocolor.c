#include "graph.h"
#include <stdio.h>

int main() {
    graph g;
    initialize_graph(&g, false);
    read_graph(&g, false);
    printf("graph\n");
    print_graph(&g);

    printf("two color result \n");
    initialize_search(&g);
    printf("%d\n", twocolor(&g));

    free_graph(&g);
    return 0;
}
