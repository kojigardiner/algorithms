// Interface for a network flow dataype with flow_edges. Supports calculation
// of maxflow and mincut after initialization.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"
#include "../flow_edge/flow_edge.h"

// flow_network data type
typedef struct flow_network flow_network_t;

// Creates and returns a pointer to a new flow_network with num_v vertices.
flow_network_t *flow_network_init(int num_v);

// Returns the number of vertices in the flow_network.
int flow_network_V(flow_network_t *f);

// Returns the number of edges in the flow_network.
int flow_network_E(flow_network_t *f);

// Adds an edge to the network. Returns true if successful, false otherwise.
bool flow_network_add_edge(flow_network_t *f, flow_edge_t *e);

// Iterator for examining edges adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool flow_network_adj_iter_init(flow_network_t *f, int v);
// Returns true if there are adjacent edges remaining, false otherwise.
bool flow_network_adj_iter_has_next(flow_network_t *f, int v);
// Copies the next adjacent edge to e. Returns true if successful, false
// otherwise.
bool flow_network_adj_iter_next(flow_network_t *g, int v, flow_edge_t **e);

// Iterator for examining all edges in the flow_network.
// Initializes an iterator. Returns true if successful, false otherwise.
bool flow_network_edge_iter_init(flow_network_t *f);
// Returns true if there are edges remaining, false otherwise.
bool flow_network_edge_iter_has_next(flow_network_t *f);
// Copies the next edge pointer to e. Returns true if successful, false
// otherwise.
bool flow_network_edge_iter_next(flow_network_t *f, flow_edge_t **e);

// Prints the contents of the flow_network to stdout.
void flow_network_print(flow_network_t *f);

// Frees memory allocated to the weighted_graph.
void flow_network_free(flow_network_t *f);

// Computes maxflow from source s to sink t for a given network. Returns the
// maximum flow through the network.
double flow_network_compute_maxflow(flow_network_t *f, int s, int t);

// Returns true if the given vertex v is on the source side of the mincut
// of the flow network with source s and sink t.
bool flow_network_incut(flow_network_t *f, int s, int t, int v);