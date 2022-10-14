// Interface for a generic bag data type with iteration support. No
// guarantee is made as to the order in which elements in the bag will be 
// iterated.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Generic bag data type.
typedef struct bag bag_t;

// Creates an empty bag that holds elements of a given item_size. Returns a
// pointer to the bag. Must call bag_free() when finished using the bag.
bag_t *bag_init(size_t item_size);

// Adds an item to the bag.
void bag_add(bag_t *b, void *item);

// Returns true if the bag is empty, false otherwise.
bool bag_is_empty(bag_t *b);

// Returns the number of items in the bag.
unsigned int bag_size(bag_t *b);

// Frees memory associated with bag.
void bag_free(bag_t *b);

// Initializes an iterator for examining contents of the bag.
void bag_iter_init(bag_t *b);

// Returns true if the iterator has more items, false otherwise.
bool bag_iter_has_next(bag_t *b);

// Copies the next item in the bag to the memory address of item.
void bag_iter_next(bag_t *b, void *item);
