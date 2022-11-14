// Interface for a generic priority queue.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "../lib/lib.h"

// Generic priority_queue data type.
typedef struct pq pq_t;

// Creates an empty MAX priority queue that holds n elements of a given
// item_size, and uses the less function to compare them.
// Returns a pointer to the priority queue. Must call pq_free() when finished 
// using the priority queue.
pq_t *max_pq_init(size_t item_size, size_t max, bool (*less)(void *, void *));

// Inserts an item into the priority queue.
bool pq_insert(pq_t *pq, void *item);

// Finds the item with the max value from the priority queue and copies it to
// the memory address of item.
// Returns true if successful, false otherwise.
bool pq_max(pq_t *pq, void *item);

// Removes the item with the max value from the priority queue and copies it to
// the memory address of item.
// Returns true if successful, false otherwise.
bool pq_del_max(pq_t *pq, void *item);

// Returns true if the priority queue is empty, false otherwise.
bool pq_is_empty(pq_t *pq);

// Returns the number of items in the priority queue.
size_t pq_size(pq_t *pq);

// Frees memory associated with the priority queue.
void pq_free(pq_t *pq);
