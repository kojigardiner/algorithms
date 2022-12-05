// Interface for finding connected components in a graph. If the graph is a 
// digraph, finds strongly connected components instead.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../graph/graph.h"
#include "../lib/lib.h"

// Cycle data type
typedef struct cc cc_t;

// Creates and returns a pointer to a new connected components struct with a 
// given graph.
cc_t *cc_init(graph_t *g);

// Returns the number of connected components in a graph.
int cc_count(cc_t *cc);

// Returns the id of hte connected components for vertex v.
int cc_id(cc_t *cc, int v);

// Returns true if v and w are part of the same connected component, false
// otherwise.
bool cc_connected(cc_t *cc, int v, int w);

// Frees memory allocated to the connected components struct.
void cc_free(cc_t *cc);
