// Interface for a generic priority queue.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

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

// Comparison functions for commmon types. Comparison functions for more
// complex types must be provided by the client.
bool less_int(void *v, void *w);
bool less_uint(void *v, void *w);
bool less_float(void *v, void *w);
bool less_double(void *v, void *w);
bool less_char(void *v, void *w);
bool less_str(void *v, void *w);