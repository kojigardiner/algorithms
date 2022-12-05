// Interface for performing topological sort on a graph.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../graph/graph.h"
#include "../lib/lib.h"

// Topological sort data type
typedef struct top_sort top_sort_t;

// Creates and returns a pointer to a new topological sort struct with a given 
// graph.
top_sort_t *top_sort_init(graph_t *g);

// Frees memory allocated to the topological sort structure.
void top_sort_free(top_sort_t *ts);

// Initialize an iterator for the topologically sorted graph.
bool top_sort_iter_init(top_sort_t *ts);

// Returns true if the iterator has a next item.
bool top_sort_iter_has_next(top_sort_t *ts);

// Copies the next value from the iterator to v.
bool top_sort_iter_next(top_sort_t *ts, int *v);
