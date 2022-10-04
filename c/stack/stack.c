#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"

// Generic node for the underlying linked list data structure.
typedef struct node {
  void *item;
  struct node *next;
} node_t;

// Generic stack data type.
typedef struct stack {
  size_t item_size;   // size of item type stored on the stack
  unsigned int n;     // number of items currently on the stack
  node_t *first;      // pointer to first node
  node_t *curr_iter;  // iterator pointer
} my_stack_t;

// Creates an empty stack that holds elements of a given item_size. Returns a
// pointer to the stack. Must call stack_free() when finished using the stack.
my_stack_t *stack_init(size_t item_size) {
  my_stack_t *s = malloc(sizeof(my_stack_t));

  s->item_size = item_size;
  s->n = 0;
  s->first = NULL;
  s->curr_iter = NULL;

  return s;
}

// Pushes an item onto the stack.
void stack_push(my_stack_t *s, void *item) {
  node_t *new_node = malloc(sizeof(node_t));
  if (!new_node) {
    printf("Failed to malloc node\n");
    exit(1);
  }
  new_node->item = malloc(s->item_size);
  if (!new_node->item) {
    printf("Failed to malloc new item\n");
    exit(1);
  }
  memcpy(new_node->item, item, s->item_size);

  // Add to front of the list
  new_node->next = s->first;
  s->first = new_node;
  s->n++;
}

// Pops an item from the stack and copies it to the memory address of item.
// Returns true if successful, false otherwise.
bool stack_pop(my_stack_t *s, void *item) {
  if (stack_is_empty(s)) {
    return false;
  }
  memcpy(item, s->first->item, s->item_size);

  node_t *old_first = s->first;

  s->first = s->first->next;

  free(old_first->item);
  free(old_first);

  s->n--;
  return true;
}

// Returns true if the stack is empty, false otherwise.
bool stack_is_empty(my_stack_t *s) {
  return (s->n == 0);
}

// Returns the number of items on the stack.
unsigned int stack_size(my_stack_t *s) {
  return s->n;
}

// Frees memory associated with stack.
void stack_free(my_stack_t *s) {
  node_t *n = s->first;
  node_t *old_n;

  while (n) {
    old_n = n;
    n = n->next;
    
    free(old_n->item);
    free(old_n);
  }

  free(s);
}

// Initializes an iterator for examining contents of the stack.
void stack_iter_init(my_stack_t *s) {
    if (stack_is_empty(s)) {
    return;
  }
  s->curr_iter = s->first;
}

// Returns true if the iterator has more items, false otherwise.
bool stack_iter_has_next(my_stack_t *s) {
  if (stack_is_empty(s)) {
    return false;
  }
  return s->curr_iter != NULL;
}

// Copies the next item in the stack to the memory address of item.
void stack_iter_next(my_stack_t *s, void *item) {
  memcpy(item, s->curr_iter->item, s->item_size);
  s->curr_iter = s->curr_iter->next;
}
