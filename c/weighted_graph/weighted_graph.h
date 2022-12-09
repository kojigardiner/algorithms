// Interface for a weighted_graph with vertices and edges.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"
#include "../edge/edge.h"

// weighted_graph data type
typedef struct weighted_graph weighted_graph_t;

// Type of weighted_graph
enum weighted_graph_type {
  UNDIRECTED,
  DIRECTED,
};

// Creates and returns a pointer to a new weighted_graph with num_v vertices.
weighted_graph_t *weighted_graph_init(int num_v, enum weighted_graph_type type);

// Creates a weighted_graph given an input file with the following format:
// num_vertices
// num_edges
// v1 w1
// v2 w2
// v3 w3
// ...
weighted_graph_t *weighted_graph_init_file(char *filename, enum weighted_graph_type type); 

// Returns a new weighted_graph that has the edges reversed. Should only be called on a 
// directed weighted_graph, on an undirected weighted_graph returns NULL.
// weighted_graph_t *weighted_graph_reverse(weighted_graph_t *g);

// Returns the number of vertices in the weighted_graph.
int weighted_graph_V(weighted_graph_t *g);

// Returns the number of edges in the weighted_graph.
int weighted_graph_E(weighted_graph_t *g);

// Adds an edge to the graph. Returns true if successful, false otherwise.
bool weighted_graph_add_edge(weighted_graph_t *g, edge_t *e);

// Iterator for examining edges adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool weighted_graph_adj_iter_init(weighted_graph_t *g, int v);
// Returns true if there are adjacent edges remaining, false otherwise.
bool weighted_graph_adj_iter_has_next(weighted_graph_t *g, int v);
// Copies the next adjacent edge to e. Returns true if successful, false
// otherwise.
bool weighted_graph_adj_iter_next(weighted_graph_t *g, int v, edge_t **e);

// Iterator for examining all edges in the graph.
// Initializes an iterator. Returns true if successful, false otherwise.
bool weighted_graph_edge_iter_init(weighted_graph_t *g);
// Returns true if there are adjacent edges remaining, false otherwise.
bool weighted_graph_edge_iter_has_next(weighted_graph_t *g);
// Copies the next adjacent edge pointer to e. Returns true if successful, false
// otherwise.
bool weighted_graph_edge_iter_next(weighted_graph_t *g, edge_t **e);

// Returns the type of weighted_graph
enum weighted_graph_type weighted_graph_type(weighted_graph_t *g);

// Prints the contents of the weighted_graph to stdout.
void weighted_graph_print(weighted_graph_t *g);

// Frees memory allocated to the weighted_graph.
void weighted_graph_free(weighted_graph_t *g);