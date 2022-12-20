// Implementation of a flow network, using an array of adjacency lists and the
// flow_edge data type.
// 
// Each index in the array represents a vertex, and each adjacency list 
// represents the edges connected to that vertex. The adjacency list is 
// implemented as a bag (unordered linked list). The network supports iteration 
// through the adjacency lists.
// 
// Maximum flow (maxflow) through the network from a given source to target
// vertex is computed using the Ford-Fulkerson algorithm. BFS is used to search 
// for "augmenting paths" from the source to target, where an augmenting path is 
// defined as a set of edges that has "residual" capacity to support changing 
// the flow. Residual capacity is supported by the flow_edge data type, which 
// calculates the delta between the given capacity of an edge and the flow that 
// can either be added (in the forward direction) or removed (in the reverse 
// direction) to alter the overall flow.
//
// If an augmenting path can be found, the algorithm then searches for the
// bottleneck capacity along the path, and updates the flow values for each
// edge on the path with that bottleneck. The algorithm then continues with
// the search for further augmenting paths until none remain.
//
// The maxflow/mincut theorem provides a simple method for determining which
// vertices lie on which side of the mincut. By simply looking at the vertices
// which were visited during the final call to has_augmenting_path, we can see
// which vertices were reachable from the source via residual capacity > 0. 
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "flow_network.h"
#include "../flow_edge/flow_edge.h"
#include "../bag/bag.h"
#include "../queue/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Private function
bool flow_network_has_augmenting_path(flow_network_t *f, int s, int t);

// Graph data type
typedef struct flow_network {
  int V;        // number of vertices
  int E;        // number of edges
  bag_t **adj;  // adjacency lists
  bag_t *edges; // bag of edges for iteration

  bool *marked;           // vertex-indexed array tracking visited vertices for maxflow
  flow_edge_t **edge_to;  // vertex-indexed array tracing an augmenting path for maxflow
  queue_t *q;             // queue used for maxflow BFS path search
  double maxflow;        // value of maximum flow

} flow_network_t;

// Creates and returns a pointer to a new flow_network with num_v vertices.
flow_network_t *flow_network_init(int num_v) {
  if (num_v <= 0) {
    return NULL;
  }
  
  flow_network_t *f = malloc(sizeof(flow_network_t));
  if (!f) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  f->V = num_v;
  f->E = 0;
  f->adj = malloc(sizeof(bag_t *) * num_v);
  if (!f->adj) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < num_v; i++) {
    f->adj[i] = bag_init(sizeof(flow_edge_t *));
  }

  f->edges = bag_init(sizeof(flow_edge_t *));

  f->marked = malloc(sizeof(bool) * num_v);
  if (!f->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  f->edge_to = malloc(sizeof(flow_edge_t *) * num_v);
  if (!f->edge_to) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  f->q = queue_init(sizeof(int));
  f->maxflow = 0;

  return f;
}

// Returns the number of vertices in the flow_network.
int flow_network_V(flow_network_t *f) {
  return f->V;
}

// Returns the number of edges in the flow_network.
int flow_network_E(flow_network_t *f) {
  return f->E;
}

// Adds an edge between vertices v and w. Returns true if successful, false
// otherwise.
bool flow_network_add_edge(flow_network_t *f, flow_edge_t *e) {
  int v, w;

  // We add the edge reference to both the adjacency list of v and w, in essence
  // creating an undirected graph.
  
  v = flow_edge_from(e);
  w = flow_edge_to(e);
  
  if (v < 0 || w < 0 || v > f->V-1 || w > f->V-1) {
    return false;
  }

  bag_add(f->adj[v], &e);
  bag_add(f->adj[w], &e);

  f->E++;

  return true;
}

// Iterator for examining edges adjacent to the given vertex v.
// Initializes an iterator. Returns true if successful, false otherwise.
bool flow_network_adj_iter_init(flow_network_t *f, int v) {
  if (v < 0 || v > f->V-1) {
    return false;
  }

  return bag_iter_init(f->adj[v]);
}

// Returns true if there are adjacent edges remaining, false otherwise.
bool flow_network_adj_iter_has_next(flow_network_t *f, int v) {
  if (v < 0 || v > f->V-1) {
    return false;
  }

  return bag_iter_has_next(f->adj[v]);
}

// Copies a pointer to the next adjacent edge to e. Returns true if successful, 
// false otherwise.
bool flow_network_adj_iter_next(flow_network_t *f, int v, flow_edge_t **e) {
  if (v < 0 || v > f->V-1) {
    return false;
  }

  bag_iter_next(f->adj[v], e);

  return true;
}

// Iterator for examining all edges.
// Initializes an iterator. Returns true if successful, false otherwise.
bool flow_network_edge_iter_init(flow_network_t *f) {
  // Fill f->edges
  flow_edge_t *e;
  for (int i = 0; i < flow_network_V(f); i++) {
    bag_iter_init(f->adj[i]);
    while (bag_iter_has_next(f->adj[i])) {
      bag_iter_next(f->adj[i], &e);
      if (flow_edge_other(e, i) > i) { // only add one copy of the edge
        bag_add(f->edges, &e);
      }
    }
  }

  return bag_iter_init(f->edges);
}

// Returns true if there are edges remaining, false otherwise.
bool flow_network_edge_iter_has_next(flow_network_t *f) {
  return bag_iter_has_next(f->edges);
}

// Copies a pointer to the next edge to e. Returns true if successful, false
// otherwise.
bool flow_network_edge_iter_next(flow_network_t *f, flow_edge_t **e) {
  bag_iter_next(f->edges, e);

  return true;
}

// Prints the contents of the flow_network to stdout.
void flow_network_print(flow_network_t *f) {
  flow_edge_t *e;
  // printf("%d vertices, %d edges\n", f->V, f->E);
  for (int v = 0; v < f->V; v++) {
    flow_network_adj_iter_init(f, v);
    while (flow_network_adj_iter_has_next(f, v)) {
      flow_network_adj_iter_next(f, v, &e);
      // Since we two references to each edge in the adjacency list (one for the
      // source and one for the sink), only print it once.
      if (flow_edge_from(e) == v) {
        flow_edge_print(e);
      }
    }
  }

  return;
}

// Frees memory allocated to the flow_network. Note that this implementation
// does not currently free edges, as that would require keeping a reference
// count for edge pointers that appear multiple times in different adjacency
// lists.
void flow_network_free(flow_network_t *f) {
  // Free each bag and its edges
  flow_edge_t *e;
  for (int i = 0; i < f->V; i++) {
    bag_iter_init(f->adj[i]);
    while (bag_iter_has_next(f->adj[i])) {
      bag_iter_next(f->adj[i], &e);
      // Could error due to double-free here
      // flow_edge_free(e);
    }
    bag_free(f->adj[i]);
  }

  bag_free(f->edges);

  // Free the list of bags
  free(f->adj);

  // Free the arrays for maxflow
  free(f->marked);
  free(f->edge_to);
  queue_free(f->q);

  // Free the flow_network
  free(f);

  return;
}

// Returns true if there is an augmenting path in the network. Uses
// breadth-first search while keeping track of visited vertices.
// Starting at the source, do BFS by looking at all adjacent edges and 
// adding to the queue any that have not yet been visited and have residual
// capacity > 0.
bool flow_network_has_augmenting_path(flow_network_t *f, int s, int t) {
  // Clear the marked array
  for (int i = 0; i < f->V; i++) {
    f->marked[i] = false;
    f->edge_to[i] = NULL;
  }

  // Enqueue the source.
  queue_enqueue(f->q, &s);
  f->marked[s] = true;
  int v;

  while (!queue_is_empty(f->q)) {
    queue_dequeue(f->q, &v);

    // Iterate over adjacent edges
    flow_network_adj_iter_init(f, v);
    flow_edge_t *e;
    while (flow_network_adj_iter_has_next(f, v)) {
      flow_network_adj_iter_next(f, v, &e);
      int w = flow_edge_other(e, v);
      if (!f->marked[w] && flow_edge_residual_capacity_to(e, w) > 0) {
        queue_enqueue(f->q, &w);
        f->marked[w] = true;
        f->edge_to[w] = e;
      }
    }
  }

  // If we ended up visiting the target, there is an augmenting path.
  return f->marked[t];
}

// Computes maxflow from source s to sink t for a given network. Uses the
// Ford-Fulkerson algorithm. Returns the maximum flow value.
double flow_network_compute_maxflow(flow_network_t *f, int s, int t) {
  f->maxflow = 0;

  // Set flow to zero for all edges.
  flow_network_edge_iter_init(f);
  flow_edge_t *e;
  while(flow_network_edge_iter_has_next(f)) {
    flow_network_edge_iter_next(f, &e);
    // Negate the existing flow by adding residual flow equal to existing flow
    // to the source vertex.
    flow_edge_add_residual_flow_to(e, flow_edge_from(e), flow_edge_flow(e));
  }

  // While an augmenting path continues to exist, add flow equivalent to the
  // bottleneck residual capacity to all edges on that path.
  double bottleneck;
  int vertex;
  while (flow_network_has_augmenting_path(f, s, t)) {
    // Trace the path from t back to s in order to find the min capacity
    vertex = t;
    bottleneck = INFINITY;
    while (vertex != s) {
      // printf("vertex %d, bottleneck %f\n", vertex, bottleneck);
      flow_edge_t *e = f->edge_to[vertex];
      double capacity = flow_edge_residual_capacity_to(e, vertex);
      if (capacity < bottleneck) {
        bottleneck = capacity;
      }
      vertex = flow_edge_other(e, vertex);
    }

    // Update flows with the bottleneck
    vertex = t;
    while (vertex != s) {
      flow_edge_t *e = f->edge_to[vertex];
      flow_edge_add_residual_flow_to(e, vertex, bottleneck);
      vertex = flow_edge_other(e, vertex);
    }
    f->maxflow += bottleneck;
  }

  return f->maxflow;
}

// Returns true if the given vertex v is on the source side of the mincut
// of the flow network with source s and sink t. Computes the maxflow with
// the given s and t vertices, then searches the marked[] array to determine
// if the given vertex can be reached from the source.
bool flow_network_incut(flow_network_t *f, int s, int t, int v) {
  flow_network_compute_maxflow(f, s, t);
  return f->marked[v];
}