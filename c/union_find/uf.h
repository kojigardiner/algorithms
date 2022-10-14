// Interface for a union find data type used to solve dynamic connectivity 
// problems. 
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).
#pragma once
#include <stdbool.h>

#define QUICK_UNION  // QUICK_UNION or QUICK_FIND

// Union find data type.
typedef struct uf uf_t;

// Creates a uf struct of size n and returns a pointer to it. Must call 
// uf_free() when finished using the uf struct.
uf_t *uf_init(int n);

// Adds connection between components containing p and q.
void uf_union(uf_t *uf, int p, int q);

// Returns true if p and q are in the same component, otherwise false.
bool uf_connected(uf_t *uf, int p, int q);

// Returns the number of connected components.
int uf_count(uf_t *uf);

// Frees memory allocated to a uf struct.
void uf_free(uf_t *uf);