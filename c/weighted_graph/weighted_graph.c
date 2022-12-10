// Implementation of a weighted graph, using an array of adjacency lists. 
// Each index in the array represents a vertex, and each adjacency list 
// represents the edges connected to that vertex.
//
// The adjacency list is implemented as a bag (unordered linked list). The
// graph supports iteration through the adjacency lists.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "weighted_graph.h"
#include "../bag/bag.h"
#include <stdio.h>
#include <stdlib.h>

// Graph data type
typedef struct weighted_graph {
  int V;        // number of vertices
  int E;        // number of edges
  bag_t **adj;  // adjacency lists
  bag_t *edges; // bag of edges for iteration
  enum weighted_graph_type type;  // type of graph
} weighted_graph_t;

// Creates and returns a pointer to a new weighted_graph with num_v vertices.
weighted_graph_t *weighted_graph_init(int num_v, enum weighted_graph_type type) {
  if (num_v <= 0) {
    return NULL;
  }
  
  weighted_graph_t *g = malloc(sizeof(weighted_graph_t));
  if (!g) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  g->V = num_v;
  g->E = 0;
  g->adj = malloc(sizeof(bag_t *) * num_v);
  if (!g->adj) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < num_v; i++) {
    g->adj[i] = bag_init(sizeof(edge_t *));
  }

  g->edges = bag_init(sizeof(edge_t *));

  g->type = type;

  return g;
}

// Creates a weighted_graph given an input file with the following format:
// num_vertices
// num_edges
// v1 w1
// v2 w2
// v3 w3
// ...
weighted_graph_t *weighted_graph_init_file(char *filename, enum weighted_graph_type type) {
  FILE *fp = fopen(filename, "r");
  
  // Read the size of the weighted_graph from the file
  int num_v, num_e;
  int ret = fscanf(fp, "%d", &num_v);
  if (ret != 1) {
    printf("Failed to read input file!\n");
    exit(EXIT_FAILURE);
  }
  ret = fscanf(fp, "%d", &num_e);
  if (ret != 1) {
    printf("Failed to read input file!\n");
    exit(EXIT_FAILURE);
  }

  if (num_v <= 0 || num_e <= 0) {
    return NULL;
  }

  // Initialize the weighted_graph
  weighted_graph_t *g = weighted_graph_init(num_v, type);

  // Fill the weighted_graph
  int v, w;
  double weight;
  for (int i = 0; i < num_e; i++) {
    ret = fscanf(fp, "%d %d %lf", &v, &w, &weight);
    if (ret != 3) {
      printf("Failed to read input file!\n");
      exit(EXIT_FAILURE);
    }
    edge_t *e = edge_init(v, w, weight);
    if (!weighted_graph_add_edge(g, e)) {
      printf("Failed to add edge!\n");
      exit(EXIT_FAILURE);
    }
  }

  return g;
}

// Returns the number of vertices in the weighted_graph.
int weighted_graph_V(weighted_graph_t *g) {
  return g->V;
}

// Returns the number of edges in the weighted_graph.
int weighted_graph_E(weighted_graph_t *g) {
  return g->E;
}

// Adds an edge between vertices v and w. Returns true if successful, false
// otherwise.
bool weighted_graph_add_edge(weighted_graph_t *g, edge_t *e) {
  int v, w;

  // For an undirected graph, we need to use the either/other method to get
  // the vertex indices to insert into the adjacency list. We add the edge
  // reference to both the adjacency list of v and w.
  if (g->type == UNDIRECTED) {
    v = edge_either(e);
    w = edge_other(e, v);
    
    if (v < 0 || w < 0 || v > g->V-1 || w > g->V-1) {
      return false;
    }

    bag_add(g->adj[v], &e);
    bag_add(g->adj[w], &e);

  // For a directed graph, we only add the edge to the source vertex's adjacency
  // list.
  } else if (g->type == DIRECTED) {
    v = edge_from(e);

    if (v < 0 || v > g->V-1) {
      return false;
    }

    bag_add(g->adj[v], &e);
  }

  g->E++;

  return true;
}

// Iterator for examining edges adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool weighted_graph_adj_iter_init(weighted_graph_t *g, int v) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  return bag_iter_init(g->adj[v]);
}

// Returns true if there are adjacent edges remaining, false otherwise.
bool weighted_graph_adj_iter_has_next(weighted_graph_t *g, int v) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  return bag_iter_has_next(g->adj[v]);
}

// Copies a pointer to the next adjacent edge to e. Returns true if successful, 
// false otherwise.
bool weighted_graph_adj_iter_next(weighted_graph_t *g, int v, edge_t **e) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  bag_iter_next(g->adj[v], e);

  return true;
}

// Iterator for examining all edges.
// Initializes an iterator. Returns true if successful, false otherwise.
bool weighted_graph_edge_iter_init(weighted_graph_t *g) {
  // Fill g->edges
  edge_t *e;
  for (int i = 0; i < weighted_graph_V(g); i++) {
    bag_iter_init(g->adj[i]);
    while (bag_iter_has_next(g->adj[i])) {
      bag_iter_next(g->adj[i], &e);
      if (edge_other(e, i) > i) { // only add one copy of the edge
        bag_add(g->edges, &e);
      }
    }
  }

  return bag_iter_init(g->edges);
}

// Returns true if there are edges remaining, false otherwise.
bool weighted_graph_edge_iter_has_next(weighted_graph_t *g) {
  return bag_iter_has_next(g->edges);
}

// Copies a pointer to the next edge to e. Returns true if successful, false
// otherwise.
bool weighted_graph_edge_iter_next(weighted_graph_t *g, edge_t **e) {
  bag_iter_next(g->edges, e);

  return true;
}


// Returns a new weighted_graph that has the edges reversed. Should only be called on a 
// directed weighted_graph, on an undirected weighted_graph returns NULL.
// weighted_graph_t *weighted_graph_reverse(weighted_graph_t *g) {
//   if (g->type == UNDIRECTED) {
//     return NULL;
//   }

//   weighted_graph_t *g_r = weighted_graph_init(weighted_graph_V(g), DIRECTED);
//   for (int v = 0; v < weighted_graph_V(g); v++) {
//     weighted_graph_adj_iter_init(g, v);
//     int w;
//     while (weighted_graph_adj_iter_has_next(g, v)) {
//       weighted_graph_adj_iter_next(g, v, &w);
//       weighted_graph_add_edge(g_r, w, v);
//     }
//   }

//   return g_r;
// }

// Returns the type of weighted_graph
enum weighted_graph_type weighted_graph_type(weighted_graph_t *g) {
  return g->type;
}

// Prints the contents of the weighted_graph to stdout.
void weighted_graph_print(weighted_graph_t *g) {
  edge_t *e;
  printf("%d vertices, %d edges\n", g->V, g->E);
  for (int v = 0; v < g->V; v++) {
    printf("%d: ", v);
    weighted_graph_adj_iter_init(g, v);
    while (weighted_graph_adj_iter_has_next(g, v)) {
      weighted_graph_adj_iter_next(g, v, &e);
      double weight = edge_weight(e);
      int from, to;
      if (g->type == UNDIRECTED) {
        from = edge_either(e); 
        to = edge_other(e, from);
      } else {
        from = edge_from(e);
        to = edge_to(e);
      }
      printf("%d-%d(%.2lf) ", from, to, weight);
    }
    printf("\n");
  }

  return;
}

// Frees memory allocated to the weighted_graph. Note that this implementation
// does not currently free edges, as that would require keeping a reference
// count for edge pointers that appear multiple times in different adjacency
// lists.
void weighted_graph_free(weighted_graph_t *g) {
  // Free each bag and its edges
  edge_t *e;
  for (int i = 0; i < g->V; i++) {
    bag_iter_init(g->adj[i]);
    while (bag_iter_has_next(g->adj[i])) {
      bag_iter_next(g->adj[i], &e);
      // Could error due to double-free here
      // edge_free(e);
    }
    bag_free(g->adj[i]);
  }

  bag_free(g->edges);

  // Free the list of bags
  free(g->adj);

  // Free the weighted_graph
  free(g);

  return;
}
