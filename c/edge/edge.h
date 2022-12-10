// Interface for a weighted edge data type.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"

// Edge data type
typedef struct edge edge_t;

// Creates and returns a pointer to a new edge with vertices v and w and weight.
edge_t *edge_init(int v, int w, double weight);

// Returns one of the vertices.
// Used for edges in undirected graphs.
int edge_either(edge_t *e);

// Returns the vertex opposite the vertex passed in.
// Used for edges in undirected graphs.
int edge_other(edge_t *e, int v);

// Returns the source vertex.
// Used for edges in directed graphs.
int edge_from(edge_t *e);

// Returns the sink vertex.
 //Used for edges in directed graphs.
int edge_to(edge_t *e);

// Returns the weight of an edge.
double edge_weight(edge_t *e);

// Comparison function for two edges, returns -1 if e1 is less than e2, 0 if
// they are the same, or +1 if e1 is larger than e2.
int edge_compare(edge_t *e1, edge_t *e2);
bool edge_less(void *e1, void *e2);

// Prints the edge vertices and weight.
void edge_print(edge_t *e);

// Frees memory associated with the edge.
void edge_free(edge_t *e);