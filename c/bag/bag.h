// Implementation of a generic bag data type with iteration support. No
// guarantee is made as to the order in which elements in the bag will be 
// iterated. Uses a singly linked list as a the underlying data structure.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Generic bag data type
typedef struct bag bag_t;

// Creates an empty bag to store elements of a given item_size and returns a
// pointer to it
bag_t *bag_init(size_t item_size);

// Adds an item to the bag
void bag_add(bag_t *b, void *item);

// Returns if the bag is empty
bool bag_is_empty(bag_t *b);

// Returns number of items in the bag
int bag_size(bag_t *b);

// Free memory associated with bag
void bag_free(bag_t *b);

// Initializes iterator
void bag_iter_init(bag_t *b);

// Returns true if iterator has more items
bool bag_iter_has_next(bag_t *b);

// Copies the next item
void bag_iter_next(bag_t *b, void *item);

// Gets the item at the currennt iterator position and copies it to item
void bag_iter_get(bag_t *b, void *item);