#include "graph.h"

#include <stdio.h>
#include <stdlib.h>


void initialize_graph(graph *g, bool directed) {
    int i;
    g->nvertices = 0;
    g->nedges = 0;
    g->directed = directed;
    for (i = 1; i <= MAXV; i++) {
        g->degree[i] = 0;
        g->edges[i] = NULL;
    }
}

void read_graph(graph *g, bool directed) {
    int i;
    int m;
    int x, y;
    initialize_graph(g, directed);
    scanf("%d %d", &(g->nvertices), &m);
    for (i = 1; i <= m; i++) {
        scanf("%d %d", &x, &y);
        insert_edge(g, x, y, directed);
    }
}

void insert_edge(graph *g, int x, int y, bool directed) {
    //edgenode constructor
    edgenode *p;
    p = malloc( sizeof( edgenode ) );
    p->weight = 0;
    p->y = y;
    //edge will insert in begin of adj list
    p->next = g->edges[x];
    //edge insert
    g->edges[x] = p;
    g->degree[x]++;
    if (!directed) {
        insert_edge(g, y, x, true);
    }
    else {
        //for not directed enter here then inserts second edge
        g->nedges++;
    }
}

void print_graph(const graph *g) {
    int i;
    edgenode *p;
    for (i = 1; i <= g->nvertices; i++) {
        printf("%d: ", i);
        p = g->edges[i];
        while (p != NULL) {
            printf(" %d", p->y);
            p = p->next;
        }
        printf("\n");
    }
}

