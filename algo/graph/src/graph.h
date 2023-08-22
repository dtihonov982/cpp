/*
    The Algorithm Design Manual 2nd ed. 2008 Edition by Steven S S. Skiena 
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "queue.h"

#define MAXV 1000

typedef struct edgenode edgenode;

typedef struct {
    edgenode *edges[MAXV + 1];
    int degree[MAXV + 1];
    int nvertices;
    int nedges;
    bool directed;
} graph;

struct edgenode {
    int y;
    int weight;
    edgenode *next;
};


void initialize_graph(graph *g, bool directed);
void free_graph(graph *g);
void read_graph(graph *g, bool directed);
void insert_edge(graph *g, int x, int y, bool directed);
void print_graph(const graph *g);
void initialize_search(const graph *g);
void bfs(const graph *g, int start);
void print_path(int start, int end);

#endif //GRAPH_H
