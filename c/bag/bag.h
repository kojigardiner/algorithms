// Interface for a generic bag data type with iteration support. No guarantee is
// made as to the order in which elements in the bag will be iterated.
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Generic bag data type
typedef struct bag bag_t;

// Creates an empty bag to store elements of a given item_size
void bag_init(bag_t *b, size_t item_size);

// Adds an item to the bag
void bag_add(bag_t *b, void *item);

// Returns if the bag is empty
bool bag_is_empty(bag_t *b);

// Returns number of items in the bag
int bag_size(bag_t *b);

// Free memory associated with bag
void bag_free(bag_t *b);

// Initializes iterator
void bag_iter_first(bag_t *b);

// Moves iterator to next item
void bag_iter_next(bag_t *b);

// Returns if we have reached the end of the iterator
bool bag_iter_done(bag_t *b);

// Gets the item at the currennt iterator position and copies it to item
void bag_iter_get(bag_t *b, void *item);