// Interface for finding single-source shortest paths in a directed
// edge-weighted graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include "../weighted_graph/weighted_graph.h"
#include <stdbool.h>
#include <math.h>

// Shortest paths data type
typedef struct sp sp_t;

// Initialize a sp struct with a given edge-weighted graph and source vertex.
// Calculates the shortest paths as part of the initialization.
sp_t *sp_init(weighted_graph_t *g, int s);

// Returns true if there is a path from the source vertex of the graph to the
// given target vertex, false otherwise.
bool sp_has_path_to(sp_t *sp, int t);

// Returns the distance from the source of the graph to the given target vertex.
// Returns INFINITY if there is no path to the target.
double sp_dist_to(sp_t *sp, int t);

// Iterator for examining the edges in the shortest path to a given target 
// vertex. Initializes the iterator, returns true if successful, false 
// otherwise.
bool sp_iter_init(sp_t *sp, int t);
// Returns true if the iterator has additional edges remaining, false otherwise.
bool sp_iter_has_next(sp_t *sp, int t);
// Copies the next pointer to the next edge in the path to e. Returns true if
// successful, false otherwise.
bool sp_iter_next(sp_t *sp, int t, edge_t **e);

// Frees memory associated with the shortest paths struct.
void sp_free(sp_t *sp);