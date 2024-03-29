// Implementation of a graph, using an array of adjacency lists. Each index in
// the array represents a vertex, and each adjacency list represents the
// vertices connected to that vertex.
//
// The adjacency list is implemented as a bag (unordered linked list). The
// graph supports iteration through the adjacency lists.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "graph.h"
#include "../bag/bag.h"
#include <stdio.h>
#include <stdlib.h>

// Graph data type
typedef struct graph {
  int V;      // number of vertices
  int E;      // number of edges
  bag_t **adj;// adjacency lists
  enum graph_type type;  // type of graph
} graph_t;

// Creates and returns a pointer to a new graph with num_v vertices.
graph_t *graph_init(int num_v, enum graph_type type) {
  if (num_v <= 0) {
    return NULL;
  }
  
  graph_t *g = malloc(sizeof(graph_t));
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
    g->adj[i] = bag_init(sizeof(int));
  }

  g->type = type;

  return g;
}

// Creates a graph given an input file with the following format:
// num_vertices
// num_edges
// v1 w1
// v2 w2
// v3 w3
// ...
graph_t *graph_init_file(char *filename, enum graph_type type) {
  FILE *fp = fopen(filename, "r");
  
  // Read the size of the graph from the file
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

  // Initialize the graph
  graph_t *g = graph_init(num_v, type);

  // Fill the graph
  int v, w;
  for (int i = 0; i < num_e; i++) {
    ret = fscanf(fp, "%d %d", &v, &w);
    if (ret != 2) {
      printf("Failed to read input file!\n");
      exit(EXIT_FAILURE);
    }
    if (!graph_add_edge(g, v, w)) {
      printf("Failed to add edge!\n");
      exit(EXIT_FAILURE);
    }
  }

  return g;
}

// Returns the number of vertices in the graph.
int graph_V(graph_t *g) {
  return g->V;
}

// Returns the number of edges in the graph.
int graph_E(graph_t *g) {
  return g->E;
}

// Adds an edge between vertices v and w. Returns true if successful, false
// otherwise.
bool graph_add_edge(graph_t *g, int v, int w) {
  if (v < 0 || w < 0 || v > g->V-1 || w > g->V-1) {
    return false;
  }

  bag_add(g->adj[v], &w);

  if (g->type == UNDIRECTED) {  
    bag_add(g->adj[w], &v);
  }

  g->E++;

  return true;
}

// Iterator for examining vertices adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool graph_adj_iter_init(graph_t *g, int v) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  return bag_iter_init(g->adj[v]);
}

// Returns true if there are adjacent vertices remaining, false otherwise.
bool graph_adj_iter_has_next(graph_t *g, int v) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  return bag_iter_has_next(g->adj[v]);
}

// Copies the next adjacent vertex to w. Returns true if successful, false
// otherwise.
bool graph_adj_iter_next(graph_t *g, int v, int *w) {
  if (v < 0 || v > g->V-1) {
    return false;
  }

  bag_iter_next(g->adj[v], w);

  return true;
}

// Returns a new graph that has the edges reversed. Should only be called on a 
// directed graph, on an undirected graph returns NULL.
graph_t *graph_reverse(graph_t *g) {
  if (g->type == UNDIRECTED) {
    return NULL;
  }

  graph_t *g_r = graph_init(graph_V(g), DIRECTED);
  for (int v = 0; v < graph_V(g); v++) {
    graph_adj_iter_init(g, v);
    int w;
    while (graph_adj_iter_has_next(g, v)) {
      graph_adj_iter_next(g, v, &w);
      graph_add_edge(g_r, w, v);
    }
  }

  return g_r;
}

// Returns the type of graph
enum graph_type graph_type(graph_t *g) {
  return g->type;
}

// Prints the contents of the graph to stdout.
void graph_print(graph_t *g) {
  printf("%d vertices, %d edges\n", g->V, g->E);
  for (int v = 0; v < g->V; v++) {
    printf("%d: ", v);
    graph_adj_iter_init(g, v);
    while (graph_adj_iter_has_next(g, v)) {
      int w;
      graph_adj_iter_next(g, v, &w);
      printf("%d ", w);
    }
    printf("\n");
  }

  return;
}

// Frees memory allocated to the graph.
void graph_free(graph_t *g) {
  // Free each bag
  for (int i = 0; i < g->V; i++) {
    bag_free(g->adj[i]);
  }
  
  // Free the list of bags
  free(g->adj);

  // Free the graph
  free(g);

  return;
}