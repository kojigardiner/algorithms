// Interface for finding a minimum spanning tree for an edge-weighted graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include "../weighted_graph/weighted_graph.h"
#include <stdbool.h>

// Available MST search methods
enum mst_type {
  PRIM,     
  KRUSKAL,  
};

// MST data type
typedef struct mst mst_t;

// Initialize an mst struct with a given edge-weighted graph and type. Cacluates
// the MST as part of the initialization.
mst_t *mst_init(weighted_graph_t *g, enum mst_type type);

// Returns the total weight of the MST.
double mst_weight(mst_t *mst);

// Iterator for examining the edges of the MST.
// Initializes the iterator, returns true if successful, false otherwise.
bool mst_iter_init(mst_t *mst);
// Returns true if the iterator has additional edges remaining, false otherwise.
bool mst_iter_has_next(mst_t *mst);
// Copies the next pointer to the next edge in the MST to e. Returns true if
// successful, false otherwise.
bool mst_iter_next(mst_t *mst, edge_t **e);

// Frees memory associated with the MST struct.
void mst_free(mst_t *mst);