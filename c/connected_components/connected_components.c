// Implementation for finding connected components in a graph. If the graph is 
// a digraph, uses the Kosaraju-Sharir algorithm to find strongly connected
// components instead. Both methods use DFS starting at each unmarked vertex.
//
// The Kosaraju-Sharir algorithm uses the following steps:
//    1. Find the reverse graph
//    2. Compute the reverse post order of the reverse graph
//    3. Run DFS on the vertices in the reverse postorder
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdio.h>
#include "connected_components.h"
#include "../topological_sort/topological_sort.h"

// Private function
void dfs_cc(cc_t *cc, int v);

// Cycle data type
typedef struct cc {
  graph_t *g;   // underlying graph
  bool *marked; // array to support DFS
  int *id;      // array of ids for each vertex
  int count;    // connected components
} cc_t;

// Creates and returns a pointer to a new connected components struct with a 
// given graph.
cc_t *cc_init(graph_t *g) {
  cc_t *cc = malloc(sizeof(cc_t));
  if (!cc) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  cc->g = g;
  cc->marked = malloc(sizeof(bool) * graph_V(g));
  if (!cc->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  cc->id = malloc(sizeof(int) * graph_V(g));
  if (!cc->id) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Initialize array
  for (int i = 0; i < graph_V(g); i++) {
    cc->marked[i] = false;
  }

  cc->count = 0;

  switch (graph_type(g)) {
    case UNDIRECTED: {
      // Run DFS starting at each vertex
      for (int v = 0; v < graph_V(g); v++) {
        if (!cc->marked[v]) {
          dfs_cc(cc, v);
          cc->count++;
        }
      }
      break;
    }
    case DIRECTED: {
      // Find strongly connected components using Kosaraju-Sharir

      // 1. Find the reverse graph
      graph_t *g_r = graph_reverse(g);

      // 2. Compute the reverse post order of the reverse graph
      top_sort_t *ts = top_sort_init(g_r);
      
      // 3. Run DFS on the vertices in the reverse postorder
      top_sort_iter_init(ts);
      int v;
      while (top_sort_iter_has_next(ts)) {
        top_sort_iter_next(ts, &v);
        if (!cc->marked[v]) {
          dfs_cc(cc, v);
          cc->count++;
        }
      }
      break;
    }
    default: {
      printf("Graph type not recognized!\n");
      exit(EXIT_FAILURE);
      break;
    }
  }

  return cc;
}

// Performs DFS on the connected component struct.
void dfs_cc(cc_t *cc, int v) {
  // Mark as visited and set the id
  cc->marked[v] = true;
  cc->id[v] = cc->count;

  graph_adj_iter_init(cc->g, v);

  int w;
  while (graph_adj_iter_has_next(cc->g, v)) {
    graph_adj_iter_next(cc->g, v, &w);
    if (!cc->marked[w]) {
      dfs_cc(cc, w);
    }
  }
}


// Returns the number of connected components in a graph.
int cc_count(cc_t *cc) {
  return cc->count;
}

// Returns the id of the connected components for vertex v.
int cc_id(cc_t *cc, int v) {
  return cc->id[v];
}

// Returns true if v and w are part of the same connected component, false
// otherwise.
bool cc_connected(cc_t *cc, int v, int w) {
  return cc->id[v] == cc->id[w];
}

// Frees memory allocated to the connected components struct.
void cc_free(cc_t *cc) {
  free(cc->id);
  free(cc->marked);
  free(cc);
}