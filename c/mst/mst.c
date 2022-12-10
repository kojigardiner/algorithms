// Implementation for finding a minimum spanning tree for an edge-weighted
// graph, using Prim's algorithm or Kruskal's algorithm.
//
// Prim's algorithm starts by "visiting" the first vertex, marking it, and 
// adding all of its adjacent edges to a min priority queue sorted by edge 
// weight. Thereafter, the min edge is removed from the priority queue, and if
// it has one vertex that has not yet been visited (and is not in the MST), it
// is visited. The algorithm ends when the priority queue is empty, or when the
// number of edges in the MST is V - 1.
//
// Note that this is the "lazy" version of Prim's algorithm, where edges in the
// priority queue may become obsolete, which is why we must check that one of
// vertices is not yet on the MST after removing it from the queue.
//
// Kruskal's algorithm inserts all edges from the graph into the priority queue,
// then considers the edges in sorted order, only adding an edge to the MST if
// it does not create a cycle. A union find data type is used to track the
// connectedness of each vertex, and can be queried to determine if a particular
// edge would connect two vertices that are already connected (hence creating)
// a cycle.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "mst.h"
#include "../bag/bag.h"
#include "../edge/edge.h"
#include "../union_find/uf.h"
#include "../priority_queue/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

// Private function
void visit(mst_t *mst, int v);

// MST data type
typedef struct mst {
  weighted_graph_t *g;  // the underlying graph
  pq_t *pq;       // priority queue of edges
  uf_t *uf;       // union find structure of vertices for Kruskal's algorithm
  bool *marked;   // array tracking visited vertices
  enum mst_type type; // type of algorithm to use
  double weight;  // total weight of MST
  bag_t *edges;   // edges in the MST
} mst_t;

// Initialize an mst struct with a given edge-weighted graph and type. Cacluates
// the MST as part of the initialization.
mst_t *mst_init(weighted_graph_t *g, enum mst_type type) {
  mst_t *mst = malloc(sizeof(mst_t));
  if (!mst) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  mst->pq = pq_init(sizeof(edge_t *), weighted_graph_E(g), MIN, edge_less);
  mst->marked = malloc(sizeof(bool) * weighted_graph_V(g));
  if (!mst->marked) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  mst->edges = bag_init(sizeof(edge_t *));
  mst->type = type;
  mst->weight = 0.0;
  mst->g = g;
  mst->uf = uf_init(weighted_graph_V(g));

  int expected_edges = weighted_graph_V(mst->g) - 1;

  if (type == PRIM) {
    edge_t *e;
    int v, w;
    
    // Visit the first vertex and add its adjacent edges to the queue.
    visit(mst, 0);

    while (!pq_is_empty(mst->pq)) {
      // Remove the edge with the lowest weight
      pq_del(mst->pq, &e);
      v = edge_either(e);
      w = edge_other(e, v);

      // If both vertices of the edge are marked, this edge is obsolete
      if (mst->marked[v] && mst->marked[w]) {
        continue;
      }

      // Add the edge to our MST and visit the unmarked vertex
      bag_add(mst->edges, &e);
      mst->weight += edge_weight(e);
      if (mst->marked[v]) {
        visit(mst, w);
      }
      if (mst->marked[w]) {
        visit(mst, v);
      }
      if (bag_size(mst->edges) == expected_edges) {
        break;
      }
    }
  } else if (type == KRUSKAL) {
    // Add all edges to the priority queue
    edge_t *e;
    weighted_graph_edge_iter_init(mst->g);
    while (weighted_graph_edge_iter_has_next(mst->g)) {
      weighted_graph_edge_iter_next(mst->g, &e);
      pq_insert(mst->pq, &e);
    }

    // Remove edges from the priority queue, and add to the MST if it does not
    // create a cycle. The UF data structure is used to determine if a cycle
    // would be created.
    int v, w;
    while (!pq_is_empty(mst->pq)) {
      // Remove the edge with the lowest weight
      pq_del(mst->pq, &e);
      v = edge_either(e);
      w = edge_other(e, v);

      // Check if the vertices are already connected. If so, this edge would
      // create a cycle, so we skip it. Otherwise, we add it to the MST and
      // create a union between their components.
      if (!uf_connected(mst->uf, v, w)) {
        uf_union(mst->uf, v, w);
        bag_add(mst->edges, &e);
        mst->weight += edge_weight(e);
      }

      if (bag_size(mst->edges) == expected_edges) {
        break;
      }
    }
  }

  return mst;
}

// Visits a vertex by marking it and adding adjacent edges to non-tree vertices
// to the priority queue.
void visit(mst_t *mst, int v) {
  mst->marked[v] = true;

  edge_t *e;
  int w;
  
  weighted_graph_adj_iter_init(mst->g, v);
  while (weighted_graph_adj_iter_has_next(mst->g, v)) {
    weighted_graph_adj_iter_next(mst->g, v, &e);
    w = edge_other(e, v);
    if (!mst->marked[w]) {
      if (!pq_insert(mst->pq, &e)) {
        printf("Failed to insert into pq!\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

// Returns the total weight of the MST.
double mst_weight(mst_t *mst) {
  return mst->weight;
}

// Iterator for examining the edges of the MST.
// Initializes the iterator, returns true if successful, false otherwise.
bool mst_iter_init(mst_t *mst) {
  return bag_iter_init(mst->edges);
}
// Returns true if the iterator has additional edges remaining, false otherwise.
bool mst_iter_has_next(mst_t *mst) {
  return bag_iter_has_next(mst->edges);
}
// Copies the next pointer to the next edge in the MST to e. Returns true if
// successful, false otherwise.
bool mst_iter_next(mst_t *mst, edge_t **e) {
  bag_iter_next(mst->edges, e);

  return true;
}

// Frees memory associated with the MST struct.
void mst_free(mst_t *mst) {
  pq_free(mst->pq);
  bag_free(mst->edges);
  uf_free(mst->uf);
  free(mst->marked);
  free(mst);

  return;
}