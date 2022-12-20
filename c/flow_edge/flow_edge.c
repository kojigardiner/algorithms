// Interface for a network flow edge data type supporting the concept of flow,
// capacity, and residual capacity. Used for maxflow/mincut calculations.
//
// The implementation is similar to that of an edge with implicit direction
// (v->w). Flow and capacity are defined in the "forward" v->w direction, while
// residual capacity takes into account the "available" flow that can be
// used in maxflow calculations. In the forward direction, residual capacity is
// simply capacity - flow. In the reverse direction, residual capacity = flow.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "flow_edge.h"
#include <stdio.h>
#include <stdlib.h>

// Edge data type
typedef struct flow_edge {
  int v;  // source vertex
  int w;  // sink vertex
  double capacity;  // flow capacity
  double flow;      // current flow
} flow_edge_t;

// Creates and returns a pointer to a new flow edge with vertices v and w and
// capacity.
flow_edge_t *flow_edge_init(int v, int w, double capacity) {
  flow_edge_t *e = malloc(sizeof(flow_edge_t));
  if (!e) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  e->v = v;
  e->w = w;
  e->capacity = capacity;
  e->flow = 0.0;

  return e;
}

// Returns the vertex opposite the vertex passed in.
int flow_edge_other(flow_edge_t *e, int vertex) {
  if (vertex == e->v) {
    return e->w;
  } else if (vertex == e->w) {
    return e->v;
  } else {
    printf("Unexpected vertex %d\n!", vertex);
    exit(EXIT_FAILURE);
  }
}

// Returns the source vertex.
int flow_edge_from(flow_edge_t *e) {
  return e->v;
}

// Returns the sink vertex.
int flow_edge_to(flow_edge_t *e) {
  return e->w;
}

// Returns the capacity of an edge.
double flow_edge_capacity(flow_edge_t *e) {
  return e->capacity;
}

// Returns the flow through an edge.
double flow_edge_flow(flow_edge_t *e) {
  return e->flow;
}

// Returns residual capacity towards vertex. Residual flow refers to the amount
// of flow available given the capacity and existing flow. In the "forward"
// v->w direction, this is simply the difference betweeen the capacity and 
// current flow. In the reverse w->v direction, this is the existing flow that
// could potentially be subtracted away.
double flow_edge_residual_capacity_to(flow_edge_t *e, int vertex) {
  if (vertex == e->w) {  // forward residual capacity
    return e->capacity - e->flow;
  } else if (vertex == e->v) {  // reverse residual capacity
    return e->flow;
  } else {
    printf("Unexpected vertex %d\n!", vertex);
    exit(EXIT_FAILURE);
  }
}

// Adds delta flow toward vertex v.
void flow_edge_add_residual_flow_to(flow_edge_t *e, int vertex, double delta) {
  if (vertex == e->w) {         // forward residual flow
    e->flow += delta;
  } else if (vertex == e->v) {  // reverse residual flow
    e->flow -= delta;
  } else {
    printf("Unexpected vertex %d\n!", vertex);
    exit(EXIT_FAILURE);
  }
}

// Prints the flow edge vertices, capacity, and flow.
void flow_edge_print(flow_edge_t *e) {
  printf("%d->%d %f %f\n", e->v, e->w, e->capacity, e->flow);
}

// Frees memory associated with the flow edge.
void flow_edge_free(flow_edge_t *e) {
  free(e);
}
