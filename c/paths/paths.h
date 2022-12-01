// Interface for finding paths in a graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "../graph/graph.h"
#include <stdbool.h>

// Available path search methods
enum paths_type {
  DFS,  // depth-first search
  BFS,  // breadth-first search
};

// Paths data type
typedef struct paths paths_t;

// Initialize a paths struct for finding all paths in a graph g from a vertex s.
paths_t *paths_init(graph_t *g, int s, enum paths_type type);

// Returns true if there exists a path from the initial vertex s in graph g to
// the given vertex v. Returns false otherwise.
bool has_path_to(paths_t *p, int v);

// Initializes an iterator for tracing the path to vertex v. Returns true if
// successful, false otherwise.
bool path_to_iter_init(paths_t *p, int v);

// Returns true if the iterator has additional elements remaining, false
// otherwise.
bool path_to_iter_has_next(paths_t *p, int v);

// Copies the next vertex on the path to w. Returns true if successful, false
// otherwise.
bool path_to_iter_next(paths_t *p, int v, int *w);

// Frees memory associated with the paths struct.
void paths_free(paths_t *p);