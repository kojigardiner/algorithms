// Implementation for finding cycles in a graph. Uses DFS.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdbool.h>
#include <stdio.h>
#include "cycle.h"
#include "../stack/stack.h"

// Private function prototypes
void dfs(cycle_t *c, int from, int v);

// Cycle data type
typedef struct cycle {
  graph_t *g;   // graph
  int *edge_to; // array pointing to previous edges on path
  bool *marked; // array keeping track of marked/visited vertices
  my_stack_t *stack; // stack for tracking directed cycles
  bool *on_stack;   // track vertices on the current stack
  bool has_cycle;
} cycle_t;

// Creates and returns a pointer to a new cycle struct with a given graph.
// Performs DFS to find a cycle and sets the instance variable has_cycle to
// true if found.
cycle_t *cycle_init(graph_t *g) {
  if (!g) {
    return NULL;
  }
  cycle_t *c = malloc(sizeof(cycle_t));
  if (!c) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  c->g = g;
  c->has_cycle = false;

  c->edge_to = malloc(sizeof(int) * graph_V(g));
  if (!c->edge_to) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  c->marked = malloc(sizeof(bool) * graph_V(g));
  if (!c->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  c->on_stack = malloc(sizeof(bool) * graph_V(g));
  if (!c->on_stack) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Initialize array
  for (int i = 0; i < graph_V(g); i++) {
    c->edge_to[i] = -1;
    c->marked[i] = false;
    c->on_stack[i] = false;
  }

  c->stack = stack_init(sizeof(int));

  // Run DFS from each unmarked vertex to check for cycles
  for (int v = 0; v < graph_V(g); v++) {
    if (!c->marked[v]) {
      dfs(c, -1, v);
    }
  }

  return c;
}

// Returns true if the graph has a cycle, false otherwise.
bool has_cycle(cycle_t *c) {
  return c->has_cycle;
}

// Frees memory allocated to the cycle.
void cycle_free(cycle_t *c) {
  free(c->marked);
  free(c->on_stack);
  stack_free(c->stack);
  free(c);
  return;
}

// Depth-first search: recursively searches for the first cycle in the graph 
// associated with c.
void dfs(cycle_t *c, int from, int v) {
  if (v < 0 || v > graph_V(c->g)-1) {
    return;
  }

  c->marked[v] = true;  // mark the vertex as visited
  
  // Iterate over adjacent vertices
  if (!graph_adj_iter_init(c->g, v)) {
    return;
  }

  c->on_stack[v] = true;    // mark it as on the stack

  int w;
  while (graph_adj_iter_has_next(c->g, v)) {
    if (c->has_cycle) {   // if we've already found a cycle, return
      return;
    }

    graph_adj_iter_next(c->g, v, &w);
    // Visit the vertex if it has not yet been visited
    if (!c->marked[w]) {
      c->edge_to[w] = v;    // set the edge we are visiting from
      dfs(c, v, w);         // visit the vertex
    } else {
      // If the vertex has already been marked, we have more to check:
      // 1) For an undirected graph, we simply check if the marked vertex is one
      // we just came from, and if not, we have found a cycle.
      // 2) For a directed graph, we need to check if the marked vertex is part
      // of the current stack.
      switch (graph_type(c->g)) {
        case UNDIRECTED:
          if (from != w) {
            c->has_cycle = true;
            return;
          }
          break;
        case DIRECTED: 
          if (c->on_stack[w]) {
            c->has_cycle = true;
            // Push onto the stack the vertices along the edge_to path
            int x = v;  // save the first vertex
            while (v != w) {
              stack_push(c->stack, &v);
              v = c->edge_to[v];
            }
            // Push the final vertex
            stack_push(c->stack, &w);
            // Push the first vertex to close the cycle
            stack_push(c->stack, &x);

            return;
          }
          break;
        default:
          printf("Graph type not recognized!\n");
          exit(EXIT_FAILURE);
      }
    }
  }

  // Remove the vertex as we are no longer considering it as part of a cycle
  c->on_stack[v] = false;

  return;
}

// Initialize an iterator for the cycle.
bool cycle_iter_init(cycle_t *c) {
  if (!c->has_cycle) {
    return false;
  }
  stack_iter_init(c->stack);
  return true;
}

// Returns true if the iterator has a next item.
bool cycle_iter_has_next(cycle_t *c) {
  return (stack_iter_has_next(c->stack));
}

// Copies the next value from the iterator to v.
bool cycle_iter_next(cycle_t *c, int *v) {
  stack_iter_next(c->stack, v);
  return true;
}