// Implementation of a weighted edge data type.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "edge.h"
#include <stdio.h>
#include <stdlib.h>

// Edge data type
typedef struct edge {
  int v;
  int w;
  double weight;
} edge_t;

// Creates and returns a pointer to a new edge with vertices v and w and weight.
edge_t *edge_init(int v, int w, double weight) {
  edge_t *e = malloc(sizeof(edge_t));
  if (!e) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  e->v = v;
  e->w = w;
  e->weight = weight;

  return e;
}

// Returns one of the vertices.
// Used for edges in undirected graphs.
int edge_either(edge_t *e) {
  return e->v;
}

// Returns the vertex opposite the vertex passed in.
// Used for edges in undirected graphs.
int edge_other(edge_t *e, int v) {
  if (v == e->v) {
    return e->w;
  } else if (v == e->w) {
    return e->v;
  } else {
    printf("Vertex unrecognized!\n");
    exit(EXIT_FAILURE);
  }
}

// Returns the source vertex.
// Used for edges in directed graphs.
// Assumes v is the source and w is the sink.
int edge_from(edge_t *e) {
  return e->v;
}

// Returns the sink vertex.
 //Used for edges in directed graphs.
 // Assumes v is the source and w is the sink.
int edge_to(edge_t *e) {
  return e->w;
}

// Returns the weight of an edge.
double edge_weight(edge_t *e) {
  return e->weight;
}

// Comparison function for two edges, returns -1 if e1 is less than e2, 0 if
// they are the same, or +1 if e1 is larger than e2.
int edge_compare(edge_t *e1, edge_t *e2) {
  if (e1->weight > e2->weight) {
    return 1;
  } else if (e1->weight < e2->weight) {
    return -1;
  } else {
    return 0;
  }
}

// Frees memory associated with the edge.
void edge_free(edge_t *e) {
  free(e);
  return;
}