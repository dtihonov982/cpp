#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

bool processed[MAXV + 1];
bool discovered[MAXV + 1];
int parent[MAXV + 1];
//only dfs
bool finished;
int entry_time[MAXV + 1];
int exit_time[MAXV + 1];
int time;

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

void free_adj_list(edgenode *p) {
    edgenode *tmp;
    while (p != NULL) {
        tmp = p->next;
        free(p);
        p = tmp;
    }
}

void free_graph(graph *g) {
    int i;
    for (i = 1; i <= g->nvertices; i++) {
        free_adj_list(g->edges[i]);
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

void process_vertex_early(int v) {
    //printf("Early processing of vertex = %d\n", v);
    printf(" %d", v);
}

void process_vertex_late(int v) {
    //printf("Late processing of vertex = %d\n", v);
}

void process_edge(int v, int y) {
    //printf("Processing of edge = (%d,%d)\n", v, y);
}

void initialize_search(const graph *g) {
    int i;
    for (i = 1; i <= g->nvertices; i++) {
        processed[i] = discovered[i] = false;
        parent[i] = -1;
    }
}

void bfs(const graph *g, int start) {
    int i;
    queue q;
    int v;
    int y;
    edgenode *p;

    init_queue(&q);
    enqueue(&q, start);
    discovered[start] = true;

    while (!empty_queue(&q)) {
        v = dequeue(&q);
        process_vertex_early(v);
        processed[v] = true;
        p = g->edges[v];
        while (p != NULL) {
            y = p->y;
            if (!processed[y] || g->directed)
                process_edge(v, y);
            if (!discovered[y]) {
                enqueue(&q, y);
                discovered[y] = true;
                parent[y] = v;
            }
            p = p->next;
        }
        process_vertex_late(v);
    }
}

void print_path(int start, int end) {
    if (start == end || end == -1) {
        printf("%d", start);
    }
    else {
        print_path(start, parent[end]);
        printf(" %d", end);
    }
}

void connected_components(const graph *g) {
    int c;
    int i;

    initialize_search(g);

    c = 0;
    for (i = 1; i <= g->nvertices; i++) {
        if (!discovered[i]) {
            c++;
            printf("Component %d: ", c);
            bfs(g, i);
            printf("\n");
        }
    }
}

void dfs(const graph *g, int v) {
    edgenode *p;
    int y;
    if (finished) return;
    discovered[v] = true;
    time = time + 1;
    entry_time[v] = time;
    process_vertex_early(v);
    p = g->edges[v];
    while (p != NULL) {
        y = p->y;
        if (discovered[y] == false) {
            parent[y] = v;
            process_edge(v, y);
            dfs(g, y);
        }
        else if (!processed[y] || g->directed)
            process_edge(v, y);
        if (finished) return;
        p = p->next;
    }
    process_vertex_late(v);
    time = time + 1;
    exit_time[v] = time;
    processed[v] = true;
}

