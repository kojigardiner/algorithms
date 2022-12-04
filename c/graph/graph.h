// Interface for a graph with vertices and edges.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"

// Graph data type
typedef struct graph graph_t;

// Type of graph
enum graph_type {
  UNDIRECTED,
  DIRECTED,
};

// Creates and returns a pointer to a new graph with num_v vertices.
graph_t *graph_init(int num_v, enum graph_type);

// Returns a new graph that has the edges reversed. Should only be called on a 
// directed graph, on an undirected graph returns NULL.
graph_t *graph_reverse(graph_t *g);

// Returns the number of vertices in the graph.
int graph_V(graph_t *g);

// Returns the number of edges in the graph.
int graph_E(graph_t *g);

// Adds an edge between vertices v and w. Returns true if successful, false
// otherwise.
bool graph_add_edge(graph_t *g, int v, int w);

// Iterator for examining vertices adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool graph_adj_iter_init(graph_t *g, int v);
// Returns true if there are adjacent vertices remaining, false otherwise.
bool graph_adj_iter_has_next(graph_t *g, int v);
// Copies the next adjacent vertex to w. Returns true if successful, false
// otherwise.
bool graph_adj_iter_next(graph_t *g, int v, int *w);

// Prints the contents of the graph to stdout.
void graph_print(graph_t *g);

// Frees memory allocated to the graph.
void graph_free(graph_t *g);