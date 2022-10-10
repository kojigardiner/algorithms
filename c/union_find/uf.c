// Implementation for a union find data type used to solve dynamic connectivity 
// problems. Utilizes the quick-find or weighted quick-union with path
// compression algorithm based on #define in uf.h. 
//
// Quick-find, as the name suggests, is able to find connected components in 
// constant-time. However, the union operation takes linear time, as it must
// iterate through the entire array of component ids to find the ids to change.
//
// Quick-union utilizes a forest of trees approach to connected components, 
// where each element in the component id array has a reference to another
// component id, or itself (root). Finding connected components requires finding the
// root of each tree, connecting components requires setting one of the roots
// equal to the other. 
// 
// We keep track of the number of nodes in each tree, and always append the 
// smaller tree to the larger one, in order to maintain balance of the tree. 
// This keeps the cost of the find/union operations to lg n. 
//
// We also apply path compression by setting each node equal to its root when
// find is called.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).
#include "uf.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct uf {
  int n;      // number of items
  int count;  // number of connected components
  int *id;    // array of component ids
  #ifdef QUICK_UNION
  int *sz;    // size of component for roots
  #endif
} uf_t;

// Initializes a uf struct. Caller is responsible for freeing.
uf_t *uf_init(int n) {
  if (n <= 0) {
    return NULL;
  }

  uf_t *uf = malloc(sizeof(uf_t));

  uf->n = n;
  uf->count = n;
  uf->id = calloc(n, sizeof(*uf->id));
  if (!uf->id) {
    printf("Failed to calloc\n");
    exit(1);
  }
  #ifdef QUICK_UNION
  uf->sz = calloc(n, sizeof(*uf->sz));
  if (!uf->sz) {
    printf("Failed to calloc\n");
    exit(1);
  }
  #endif

  // Set id of each object to itself (n distinct components)
  for (int i = 0; i < n; i++) {
    uf->id[i] = i;
    #ifdef QUICK_UNION
    uf->sz[i] = 1;
    #endif
  }

  return uf;
}

// Finds root of component tree
int uf_root(uf_t *uf, int i) {
  int start_idx = i;
  int root;
  while (i != uf->id[i]) {
    i = uf->id[i];
  }
  root = i;       // Save the root
  
  // Achieve path compression by resetting i and looping again, 
  // setting each node to its root
  i = start_idx;  
  while (i != uf->id[i]) {
    i = uf->id[i];
    uf->id[i] = root;
  }

  return root;
}

// Returns the component identifier for p.
int uf_find(uf_t *uf, int p) {
  #ifdef QUICK_FIND
  return uf->id[p];
  #endif
  #ifdef QUICK_UNION
  return uf_root(uf, p);
  #endif
}


// Adds connection between components containing p and q
void uf_union(uf_t *uf, int p, int q) {
  int pid = uf_find(uf, p);
  int qid = uf_find(uf, q);

  // Nothing to do if already in the same component
  if (pid == qid) {
    return;
  }

  #ifdef QUICK_FIND
  // Change all entries with id[p] to id[q]
  for (int i = 0; i < uf->n; i++) {
    if (uf->id[i] == pid) {
      uf->id[i] = qid;
    }
  }
  #endif
  #ifdef QUICK_UNION
  // Set root of one node equal to that of the other in order to connect them.
  // Check the size of each tree in order to append the smaller tree to the 
  // larger one, then update the size of the new root.
  if (uf->sz[pid] < uf->sz[qid]) {
    // p < q, so set p = q and update q's size by p's
    uf->id[pid] = uf->id[qid];
    uf->sz[qid] += uf->sz[pid];
  } else {
    uf->id[qid] = uf->id[pid];
    uf->sz[pid] += uf->sz[qid];
  }
  #endif

  uf->count--;
}

// Returns true if p and q are in the same component, otherwise false
bool uf_connected(uf_t *uf, int p, int q) {
  return (uf_find(uf, p) == uf_find(uf, q));
}

// Returns the number of connected components.
int uf_count(uf_t *uf) {
  return uf->count;
}

// Frees memory allocated to a uf struct
void uf_free(uf_t *uf) {
  free(uf->id);
  free(uf);
}