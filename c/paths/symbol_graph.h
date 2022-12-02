// Interface for a Symbol Graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdio.h>
#include <stdlib.h>
#include "../symbol_table/symbol_table.h"
#include "../graph/graph.h"

// Symbol graph data type
typedef struct sg sg_t;

// Initializes and returns a symbol graph read in from a given filename with a 
// specified delimiter between items.
sg_t *sg_init(char *filename, char *delimiter);

// Returns true if the symbol graph contains the given key, false otherwise.
bool sg_contains(sg_t *sg, char *key);

// Returns the index of the vertex with the given key in the symbol graph.
// Returns -1 if the key does not exist.
int sg_index_of(sg_t *sg, char *key);

// Returns the key at the given index. Returns NULL if the key does not exist.
char *sg_name_of(sg_t *sg, int index);

// Returns the underlying graph for the symbol graph.
graph_t *sg_graph(sg_t *sg);

// Returns the size of the symbol graph.
int sg_size(sg_t *sg);