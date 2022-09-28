#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bag.h"

typedef struct node {
  void *item;
  struct node *next;
} node_t;

typedef struct bag {
  size_t item_size;
  unsigned int n;
  node_t *first;
  node_t *curr_iter;
} bag_t;

// Creates an empty bag 
void bag_init(bag_t *b, size_t item_size) {
  b->first = NULL;
  b->curr_iter = NULL;
  b->n = 0;
  b->item_size = item_size;
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

  new_node->next = b->first;
  b->first = new_node;
  b->n++;
}

// Returns if the bag is empty
bool bag_is_empty(bag_t *b) {
  return (b->n == 0);
}

// Returns number of items in the bag
int bag_size(bag_t *b) {
  return (b->n);
}

// Initializes iterator
void bag_iter_first(bag_t *b) {
  if (bag_is_empty(b)) {
    return;
  }
  b->curr_iter = b->first;
}

// Moves iterator to next item
void bag_iter_next(bag_t *b) {
  b->curr_iter = b->curr_iter->next;
}

// Returns if we have reached the end of the iterator
bool bag_iter_done(bag_t *b) {
  return (b->curr_iter == NULL);
}

// Gets the item at the currennt iterator position and copies it to item
void bag_iter_get(bag_t *b, void *item) {
  memcpy(item, b->curr_iter->item, b->item_size);
}

// Removes one item from the bag, returns true if successful, false otherwise
// Note this is a private function, for testing only.
bool bag_remove(bag_t *b, void *item) {
  if (bag_is_empty(b)) {
    return false;
  }
  memcpy(item, b->first->item, b->item_size);
  free(b->first->item);
  b->first = b->first->next;
  b->n--;
  return true;
}

// Free memory associated with bag
void bag_free(bag_t *b) {
  node_t *n = b->first;
  node_t *last;

  while (n) {
    free(n->item);
    last = n;
    n = n->next;
    free(last);
  }
}

int main() {
  // Test a bag of ints
  bag_t b;
  int value;
  int values[] = {10, 3, 4, -11};
  bag_init(&b, sizeof(int));

  for (int i = 0; i < NELEMS(values); i++) {
    value = values[i];
    bag_add(&b, &value);
    printf("Added %d to bag\n", value);
  }

  assert(bag_size(&b) == 4);

  for (bag_iter_first(&b); !bag_iter_done(&b); bag_iter_next(&b)) {
    bag_iter_get(&b, &value);
    printf("Iterated %d in bag\n", value);
  }

  while(!bag_is_empty(&b)) {
    bag_remove(&b, &value);
    printf("Removed %d from bag\n", value);
  }

  assert(bag_is_empty(&b));

  bag_free(&b);

  // Test a bag of strings (stored on the heap)
  char *str;
  char *strs[] = {"hello", "world", "this is me", "what"};;
  bag_init(&b, sizeof(char *));

  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    bag_add(&b, &str);
    printf("Added %s to bag\n", str);
  }
  
  assert(bag_size(&b) == 4);

  for (bag_iter_first(&b); !bag_iter_done(&b); bag_iter_next(&b)) {
    bag_iter_get(&b, &str);
    printf("Iterated %s in bag\n", str);
  }

  while(!bag_is_empty(&b)) {
    bag_remove(&b, &str);
    printf("Removed %s from bag\n", str);
    free(str);
  }

  assert(bag_is_empty(&b));

  bag_free(&b);

  printf("Passed all tests.\n");

  return 0;
}