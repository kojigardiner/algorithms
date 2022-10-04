#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bag.h"

// Generic node for the underlying linked list data structure.
typedef struct node {
  void *item;
  struct node *next;
} node_t;

// Generic bag data type.
typedef struct bag {
  size_t item_size;   // size of item type stored in the bag
  unsigned int n;     // number of items currently in the bag
  node_t *first;      // pointer to first node
  node_t *curr_iter;  // iterator pointer
} bag_t;

bag_t *bag_init(size_t item_size) {
  bag_t *b = malloc(sizeof(bag_t));

  b->first = NULL;
  b->curr_iter = NULL;
  b->n = 0;
  b->item_size = item_size;
  
  return b;
}

// Adds an item to the bag
void bag_add(bag_t *b, void *item) {
  node_t *new_node = malloc(sizeof(node_t));
  if (!new_node) {
    printf("Failed to malloc node\n");
    exit(1);
  }
  new_node->item = malloc(b->item_size);
  if (!new_node->item) {
    printf("Failed to malloc new item\n");
    exit(1);
  }
  memcpy(new_node->item, item, b->item_size);

  // Add to front of the list
  new_node->next = b->first;
  b->first = new_node;
  b->n++;
}

// Returns if the bag is empty
bool bag_is_empty(bag_t *b) {
  return (b->n == 0);
}

// Returns number of items in the bag
unsigned int bag_size(bag_t *b) {
  return (b->n);
}

// Initializes iterator
void bag_iter_init(bag_t *b) {
  if (bag_is_empty(b)) {
    return;
  }
  b->curr_iter = b->first;
}

// Returns true if iterator has more items
bool bag_iter_has_next(bag_t *b) {
  if (bag_is_empty(b)) {
    return false;
  }
  return b->curr_iter != NULL;
}

// Copies the next item
void bag_iter_next(bag_t *b, void *item) {
  memcpy(item, b->curr_iter->item, b->item_size);
  b->curr_iter = b->curr_iter->next;
}

// Removes one item from the bag, returns true if successful, false otherwise
// Note this is a private function, for testing only.
bool bag_remove(bag_t *b, void *item) {
  if (bag_is_empty(b)) {
    return false;
  }
  memcpy(item, b->first->item, b->item_size);
  
  node_t *old_first = b->first;

  b->first = b->first->next;
  
  free(old_first->item);
  free(old_first);

  b->n--;
  return true;
}

// Free memory associated with bag
void bag_free(bag_t *b) {
  node_t *n = b->first;
  node_t *old_n;

  while (n) {
    old_n = n;
    n = n->next;
    free(old_n->item);
    free(old_n);
  }

  free(b);
  b = NULL;
}
