// Implementation for finding paths in a graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "../graph/graph.h"
#include "../paths/paths.h"
#include "../stack/stack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Private function prototypes
void dfs(paths_t *p, int v);

// Paths data type
typedef struct paths {
  graph_t *g;   // graph
  int s;        // source vertex
  bool *marked; // array keeping track of marked/visited vertices
  int *edge_to; // array keeping track of the previously visited vertex
  my_stack_t **stacks; // array of pointers to stacks for iteration
  enum paths_type type;   // search type
} paths_t;

// Initialize a paths struct for finding all paths in a graph g from a vertex s.
// Also accepts a type argument to specify the search method.
paths_t *paths_init(graph_t *g, int s, enum paths_type type) {
  if (!g) {
    return NULL;
  }
  if (s < 0 || s > graph_V(g)-1) {
    return NULL;
  }

  paths_t *p = malloc(sizeof(paths_t));
  if (!p) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  p->g = g;
  p->s = s;
  p->type = type;

  p->marked = malloc(sizeof(bool) * graph_V(g));
  if (!p->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  p->edge_to = malloc(sizeof(int) * graph_V(g));
  if (!p->edge_to) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  p->stacks = malloc(sizeof(stack_t *) * graph_V(g));
  if (!p->stacks) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Initialize arrays
  for (int i = 0; i < graph_V(g); i++) {
    p->marked[i] = false;
    p->edge_to[i] = -1;
    p->stacks[i] = NULL;
  }

  dfs(p, s);

  return p;
}

// Depth-first search: recursively finds paths in the graph associated with p, 
// starting at source vertex v.
void dfs(paths_t *p, int v) {
  if (v < 0 || v > graph_V(p->g)-1) {
    return;
  }

  p->marked[v] = true;  // mark the vertex as visited
  
  // Iterate over adjacent vertices
  if (!graph_adj_iter_init(p->g, v)) {
    return;
  }
  int w;
  while (graph_adj_iter_has_next(p->g, v)) {
    graph_adj_iter_next(p->g, v, &w);
    // Visit the vertex if it has not yet been visited
    if (!p->marked[w]) {
      p->edge_to[w] = v; // record where we are coming from
      dfs(p, w);         // visit the vertex
    }
  }

  return;
}

// Returns true if there exists a path from the initial vertex s in graph g to
// the given vertex v. Returns false otherwise.
bool has_path_to(paths_t *p, int v) {
  if (v < 0 || v > graph_V(p->g)-1) {
    return false;
  }
  return p->marked[v];
}

// Initializes an iterator for tracing the path to vertex v. Returns true if
// successful, false otherwise.
bool path_to_iter_init(paths_t *p, int v) {
  if (v < 0 || v > graph_V(p->g)-1) {
    return false;
  }
  if (!p->marked[v]) {
    return false;
  }

  // Create a stack for the path to this vertex
  if (p->stacks[v]) {
    free(p->stacks[v]); // free any pre-existing stack
  }
  p->stacks[v] = stack_init(sizeof(int));

  // Trace the path back to the source, pushing each vertex onto the stack
  for (int w = v; w != p->s; w = p->edge_to[w]) {
    stack_push(p->stacks[v], &w);
  }
  stack_push(p->stacks[v], &p->s);
  
  // Initialize an iterator for the stack
  stack_iter_init(p->stacks[v]);
  
  return true;
}

// Returns true if the iterator has additional elements remaining, false
// otherwise.
bool path_to_iter_has_next(paths_t *p, int v) {
  if (v < 0 || v > graph_V(p->g)-1) {
    return false;
  }

  return stack_iter_has_next(p->stacks[v]);
}

// Copies the next vertex on the path to w. Returns true if successful, false
// otherwise.
bool path_to_iter_next(paths_t *p, int v, int *w) {
  if (v < 0 || v > graph_V(p->g)-1) {
    return false;
  }

  stack_iter_next(p->stacks[v], w);

  return true;
}

// Frees memory associated with the paths struct.
void paths_free(paths_t *p) {
  for (int i = 0; i < graph_V(p->g); i++) {
    if (p->stacks[i]) {
      free(p->stacks[i]);
    }
  }
  free(p->stacks);
  free(p->edge_to);
  free(p->marked);
  free(p);
}