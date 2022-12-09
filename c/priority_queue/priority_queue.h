// Interface for a generic priority queue.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "../lib/lib.h"

// Priority queue types
enum pq_type {
  MAX,
  MIN
};

// Generic priority_queue data type.
typedef struct pq pq_t;

// Creates an empty priority queue that holds n elements of a given
// item_size, and uses the less function to compare them. The type argument
// determines if this is a MAX or MIN oriented priority queue.
// Returns a pointer to the priority queue. Must call pq_free() when finished 
// using the priority queue.
pq_t *pq_init(size_t item_size, size_t max, enum pq_type type, bool (*less)(void *, void *));

// Inserts an item into the priority queue.
bool pq_insert(pq_t *pq, void *item);

// Finds the item with the min or max value from the priority queue and copies 
// it to the memory address of item.
// Returns true if successful, false otherwise.
bool pq_front(pq_t *pq, void *item);

// Removes the item at the front of the priority queue and copies it to
// the memory address of item. This is either the max or min item depending
// on the queue type.
// Returns true if successful, false otherwise.
bool pq_del(pq_t *pq, void *item);

// Returns true if the priority queue is empty, false otherwise.
bool pq_is_empty(pq_t *pq);

// Returns the number of items in the priority queue.
size_t pq_size(pq_t *pq);

// Frees memory associated with the priority queue.
void pq_free(pq_t *pq);
