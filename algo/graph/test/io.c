#include "graph.h"

int main() {
    graph g;
    initialize_graph(&g, false);
    read_graph(&g, false);
    print_graph(&g);
    return 0;
}
