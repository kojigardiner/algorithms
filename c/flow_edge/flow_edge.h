// Interface for a network flow edge data type supporting the concept of flow,
// capacity, and residual capacity. Used for maxflow/mincut calculations.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"

// Edge data type
typedef struct flow_edge flow_edge_t;

// Creates and returns a pointer to a new flow edge with vertices v and w and
// capacity.
flow_edge_t *flow_edge_init(int v, int w, double capacity);

// Returns the vertex opposite the vertex passed in.
int flow_edge_other(flow_edge_t *e, int vertex);

// Returns the source vertex.
int flow_edge_from(flow_edge_t *e);

// Returns the sink vertex.
int flow_edge_to(flow_edge_t *e);

// Returns the capacity of an edge.
double flow_edge_capacity(flow_edge_t *e);

// Returns the flow through an edge.
double flow_edge_flow(flow_edge_t *e);

// Returns residual capacity towards vertex v.
double flow_edge_residual_capacity_to(flow_edge_t *e, int vertex);

// Adds delta flow toward vertex v.
void flow_edge_add_residual_flow_to(flow_edge_t *e, int vertex, double delta);

// Prints the flow edge vertices, capacity, and flow.
void flow_edge_print(flow_edge_t *e);

// Frees memory associated with the flow edge.
void flow_edge_free(flow_edge_t *e);