// Implementation for a generic queue data type with iteration support. Uses a 
// singly linked list as a the underlying data structure.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"

// Generic node for the underlying linked list data structure.
typedef struct node {
  void *item;
  struct node *next;
} node_t;

// Generic queue data type.
typedef struct queue {
  size_t item_size;   // size of item type stored on the queue
  unsigned int n;     // number of items currently on the queue
  node_t *first;      // pointer to first node
  node_t *last;       // pointer to last node
  node_t *curr_iter;  // iterator pointer
} queue_t;

// Creates an empty queue that holds elements of a given item_size. Returns a
// pointer to the queue. Must call queue_free() when finished using the queue.
queue_t *queue_init(size_t item_size) {
  queue_t *q = malloc(sizeof(queue_t));

  q->item_size = item_size;
  q->n = 0;
  q->first = NULL;
  q->last = NULL;
  q->curr_iter = NULL;

  return q;
}

// Enqueue an item at the back of the queue.
void queue_enqueue(queue_t *q, void *item) {
  node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) {
    printf("Failed to malloc node\n");
    exit(1);
  }
  new_node->item = malloc(q->item_size);
  if (!new_node->item) {
    printf("Failed to malloc new item\n");
    exit(1);
  }
  memcpy(new_node->item, item, q->item_size);

  if (q->n == 0) {
    q->first = new_node;
    q->last = new_node;
  } else {
    q->last->next = new_node;
    q->last = new_node;
  }

  q->n++;
}

// Dequeues an item from the front of the queue and copies it to the memory 
// address of item. Returns true if successful, false otherwise.
bool queue_dequeue(queue_t *q, void *item) {
  if (queue_is_empty(q)) {
    return false;
  }
  memcpy(item, q->first->item, q->item_size);

  node_t *old_first = q->first;

  q->first = q->first->next;
  if (!q->first) {
    q->last = NULL;
  }

  free(old_first->item);
  free(old_first);

  q->n--;

  return true;
}

// Returns true if the queue is empty, false otherwise.
bool queue_is_empty(queue_t *q) {
  return (q->n == 0);
}

// Returns the number of items in the queue.
unsigned int queue_size(queue_t *q) {
  return (q->n);
}

// Frees memory associated with queue.
void queue_free(queue_t *q) {
  node_t *n = q->first;
  node_t *old_n;

  while (n) {
    old_n = n;
    n = n->next;
    
    free(old_n->item);
    free(old_n);
  }

  free(q);
}

// Initializes an iterator for examining contents of the queue.
void queue_iter_init(queue_t *q) {
  if (queue_is_empty(q)) {
    return;
  }
  q->curr_iter = q->first;
}

// Returns true if the iterator has more items, false otherwise.
bool queue_iter_has_next(queue_t *q) {
  if (queue_is_empty(q)) {
    return false;
  }
  return q->curr_iter != NULL;
}

// Copies the next item in the queue to the memory address of item.
void queue_iter_next(queue_t *q, void *item) {
  memcpy(item, q->curr_iter->item, q->item_size);
  q->curr_iter = q->curr_iter->next;
}
