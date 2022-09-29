// Implementation of a generic queue data type with iteration support. Uses a 
// singly linked list as a the underlying data structure.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Generic queue data type.
typedef struct queue queue_t;

// Creates an empty queue that holds elements of a given item_size. Returns a
// pointer to the queue. Must call queue_free() when finished using the queue.
queue_t *queue_init(size_t item_size);

// Enqueue an item at the front of the queue.
void queue_enqueue(queue_t *q, void *item);

// Dequeues an item from the back of the queue and copies it to the memory 
// address of item. Returns true if successful, false otherwise.
bool queue_dequeue(queue_t *q, void *item);

// Returns true if the queue is empty, false otherwise.
bool queue_is_empty(queue_t *q);

// Returns the number of items in the queue.
unsigned int queue_size(queue_t *q);

// Frees memory associated with queue.
void queue_free(queue_t *q);

// Initializes an iterator for examining contents of the queue.
void queue_iter_init(queue_t *q);

// Returns true if the iterator has more items, false otherwise.
bool queue_iter_has_next(queue_t *q);

// Copies the next item in the queue to the memory address of item.
void queue_iter_next(queue_t *q, void *item);
