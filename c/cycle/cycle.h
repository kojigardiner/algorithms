// Interface for finding cycles in a graph.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../graph/graph.h"
#include "../lib/lib.h"

// Cycle data type
typedef struct cycle cycle_t;

// Creates and returns a pointer to a new cycle struct with a given graph.
cycle_t *cycle_init(graph_t *g);

// Returns true if the graph has a cycle, false otherwise.
bool has_cycle(cycle_t *c);

// Frees memory allocated to the cycle.
void cycle_free(cycle_t *c);

// Initialize an iterator for the cycle.
bool cycle_iter_init(cycle_t *c);

// Returns true if the iterator has a next item.
bool cycle_iter_has_next(cycle_t *c);

// Copies the next value from the iterator to v.
bool cycle_iter_next(cycle_t *c, int *v);