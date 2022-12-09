// Implementation for performing topological sort on a graph. Topological sort
// involves sorting the vertices of a directed graph such that all edges "point"
// upward. This is akin to solving an ordering problem with precedence
// constraints.
// 
// The algorithm visits each vertex via depth-first search, and pushes it onto
// a stack *after* the recursive DFS call returns. This "post-order" stack can
// then be iterated over to provide the topological order.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdbool.h>
#include <stdio.h>
#include "topological_sort.h"
#include "../stack/stack.h"
#include "../cycle/cycle.h"

// Private function
void dfs_reverse_postorder(top_sort_t *ts, int v);

// Topological sort data type
typedef struct top_sort {
  graph_t *g;         // underlying graph
  bool *marked;       // track visited vertices
  my_stack_t *order;  // reverse postorder (topologically sorted)
} top_sort_t;

// Creates and returns a pointer to a new topological sort struct with a given 
// graph. Returns NULL on failure.
top_sort_t *top_sort_init(graph_t *g) {
  // Topological sort is only defined on digraphs
  if (graph_type(g) != DIRECTED) {
    printf("Graph is not directed!\n");
    return NULL;
  }
  // Check if the graph is a DAG
  cycle_t *c = cycle_init(g);
  if (has_cycle(c)) {
    printf("Graph has a cycle!\n");
    // return NULL;
  }

  top_sort_t *ts = malloc(sizeof(top_sort_t));
  if (!ts) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  ts->marked = malloc(sizeof(bool) * graph_V(g));
  if (!ts->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < graph_V(g); i++) {
    ts->marked[i] = false;
  }

  ts->g = g;
  ts->order = stack_init(sizeof(int));

  // Use DFS from each vertex to compute the reverse postorder
  for (int v = 0; v < graph_V(g); v++) {
    if (!ts->marked[v]) {
      dfs_reverse_postorder(ts, v);
    }
  }

  return ts;
}

// Performs DFS on the graph starting at vertex v. Pushes vertices to the order
// instance variable stack post-DFS, which gives reverse post-order upon
// iteration.
void dfs_reverse_postorder(top_sort_t *ts, int v) {
  ts->marked[v] = true;

  graph_adj_iter_init(ts->g, v);
  int w;
  while (graph_adj_iter_has_next(ts->g, v)) {
    graph_adj_iter_next(ts->g, v, &w);
    if (!ts->marked[w]) {
      dfs_reverse_postorder(ts, w);
    }
  }

  stack_push(ts->order, &v);
}

// Frees memory allocated to the topological sort structure.
void top_sort_free(top_sort_t *ts) {
  free(ts->marked);
  stack_free(ts->order);
  free(ts);
}

// Initialize an iterator for the topologically sorted graph.
bool top_sort_iter_init(top_sort_t *ts) {
  stack_iter_init(ts->order);
  return true;
}

// Returns true if the iterator has a next item.
bool top_sort_iter_has_next(top_sort_t *ts) {
  return stack_iter_has_next(ts->order);
}

// Copies the next value from the iterator to v.
bool top_sort_iter_next(top_sort_t *ts, int *v) {
  stack_iter_next(ts->order, v);
  return true;
}