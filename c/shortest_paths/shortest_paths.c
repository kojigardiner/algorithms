// Implementation for finding single-source shortest paths in a directed
// edge-weighted graph.
//
// Uses Dijkstra's algorithm, which does *not* work for graphs with negative
// edge weights. Unlike the textbook, this implementation makes use of a vertex 
// struct with embedded dist_to and edge_to members. This enables use of a
// simple priority queue with a custom less_vertex comparator, rather than
// an indexed priority queue.
//
// The algorithm starts at the source vertex and adds it the priority queue. It
// the progressively removes the vertex with the shortest dist_to from the
// queue, and "relaxes" it. The relax operation examines all edges connected
// to the vertex, and determines if the path through the current vertex would
// reduce the dist_to for any of the vertices connected. If so, those vertices
// are updated with new edge_to and dist_to values, and added to the priority
// queue for further processing.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "shortest_paths.h"
#include "../priority_queue/priority_queue.h"
#include "../stack/stack.h"

#include <stdio.h>
#include <stdlib.h>

// Private definitions

// Vertex data type that can be stored on the priority queue
typedef struct vertex {
  int idx;          // vertex index
  double dist_to;   // distance back to the source
  edge_t *edge_to;  // pointer to an edge object back to the source
} vertex_t;

// Comparison function for vertex structs.
bool less_vertex(void *a, void *b);

// Relaxes edges connected to vertex v
void relax(sp_t *sp, int v_idx);

// Checks for negative edge weights
bool has_negative_edges(weighted_graph_t *g);

// Shortest paths data type
typedef struct sp {
  weighted_graph_t *g;  // underlying graph
  int s;                // source vertex
  vertex_t **vertices;  // vertex-indexed array of pointers to vertex structs
  pq_t *pq;             // priority queue of vertices to be examined
  my_stack_t **stacks;  // stacks for iterating over paths to each target vertex
} sp_t;

// Initialize a sp struct with a given edge-weighted graph and source vertex.
// Calculates the shortest paths as part of the initialization.
sp_t *sp_init(weighted_graph_t *g, int s) {
  if (weighted_graph_type(g) != DIRECTED) {
    printf("Graph must be directed!\n");
    return NULL;
  }

  if (has_negative_edges(g)) {
    printf("Graph cannot have negative edge weights!\n");
    return NULL;
  }

  sp_t *sp = malloc(sizeof(sp_t));
  if (!sp) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  sp->g = g;
  sp->s = s;

  sp->pq = pq_init(sizeof(vertex_t *), weighted_graph_V(g), MIN, less_vertex);
  sp->vertices = malloc(sizeof(vertex_t) * weighted_graph_V(g));
  if (!sp->vertices) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  sp->stacks = malloc(sizeof(my_stack_t *) * weighted_graph_V(g));
  if (!sp->stacks) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Initialize vertex and stacks arrays
  for (int i = 0; i < weighted_graph_V(g); i++) {
    vertex_t *v = malloc(sizeof(vertex_t));
    if (!v) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
    v->idx = i;

    if (i == s) {
      v->dist_to = 0.0;
    } else {
      v->dist_to = INFINITY;
    }
    v->edge_to = NULL;

    sp->vertices[i] = v;
    sp->stacks[i] = NULL;
  }

  // Compute the shortest paths
  vertex_t *v = sp->vertices[s];

  // Insert the source, then iterate over the queue relaxing at each vertex
  pq_insert(sp->pq, &v);
  while (!pq_is_empty(sp->pq)) {
    pq_del(sp->pq, &v);
    relax(sp, v->idx);
  }

  return sp;
}

// Returns true if there is a path from the source vertex of the graph to the
// given target vertex, false otherwise.
bool sp_has_path_to(sp_t *sp, int t) {
  return sp->vertices[t]->dist_to < INFINITY;
}

// Returns the distance from the source of the graph to the given target vertex.
// Returns INFINITY if there is no path to the target.
double sp_dist_to(sp_t *sp, int t) {
  return sp->vertices[t]->dist_to;
}

// Iterator for examining the edges in the shortest path to a given target 
// vertex. Initializes the iterator, returns true if successful, false 
// otherwise.
bool sp_iter_init(sp_t *sp, int t) {
  // If we've already created a stack for this target, free it
  if (sp->stacks[t]) {
    stack_free(sp->stacks[t]);
  }
  sp->stacks[t] = stack_init(sizeof(edge_t *));
  if (!sp->stacks[t]) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  
  // Push edges onto the stack
  vertex_t *v = sp->vertices[t];
  edge_t *e;
  while (true) {
    e = v->edge_to;
    if (e == NULL) {
      break;
    }
    stack_push(sp->stacks[t], &e);
    v = sp->vertices[edge_from(e)]; // since we're following the edges in 
                                    // reverse, we want edge_from
  }

  stack_iter_init(sp->stacks[t]);

  return true;
}

// Returns true if the iterator has additional edges remaining, false otherwise.
bool sp_iter_has_next(sp_t *sp, int t) {
  return stack_iter_has_next(sp->stacks[t]);
}

// Copies the next pointer to the next edge in the path to e. Returns true if
// successful, false otherwise.
bool sp_iter_next(sp_t *sp, int t, edge_t **e) {
  return stack_pop(sp->stacks[t], e);
}

// Frees memory associated with the shortest paths struct.
void sp_free(sp_t *sp) {
  pq_free(sp->pq);
  for (int i = 0; i < weighted_graph_V(sp->g); i++) {
    edge_t *e = sp->vertices[i]->edge_to;
    if (e != NULL) {
      edge_free(e);
    }
    free(sp->vertices[i]);
    stack_free(sp->stacks[i]);
  }
  free(sp->vertices);
  free(sp->stacks);
  free(sp);

  return;
}

// Comparison function for vertex structs.
bool less_vertex(void *a, void *b) {
  return ((vertex_t *)a)->dist_to < ((vertex_t *)b)->dist_to;
}

// Relaxes edges connected to vertex v. 
void relax(sp_t *sp, int v_idx) {
  vertex_t *v = sp->vertices[v_idx];
  vertex_t *w;

  edge_t *e;
  int w_idx;

  // Iterate over edges connected to v
  weighted_graph_adj_iter_init(sp->g, v_idx);
  while (weighted_graph_adj_iter_has_next(sp->g, v_idx)) {
    weighted_graph_adj_iter_next(sp->g, v_idx, &e);

    w_idx = edge_to(e);
    w = sp->vertices[w_idx];

    // Relax if the path through v to w is shorter than the currently known path
    // to w
    if (v->dist_to + edge_weight(e) < w->dist_to) {
      w->dist_to = v->dist_to + edge_weight(e);
      w->edge_to = e;

      // Add w to the priority queue
      pq_insert(sp->pq, &w);
    }
  }
}

// Checks for negative edge weights
bool has_negative_edges(weighted_graph_t *g) {
  weighted_graph_edge_iter_init(g);
  edge_t *e;
  while (weighted_graph_edge_iter_has_next(g)) {
    weighted_graph_edge_iter_next(g, &e);
    if (edge_weight(e) < 0) {
      return true;
    }
  }

  return false;
}