// Implementation of a generic priority queue using a binary heap. The binary 
// heap is a complete binary tree implemented as an array, with index 0 unused, 
// and index 1 being the root of the tree. Each node has a value greater than
// that of either of its children.
// 
// To find the two children of an node, go to index 2*k and 2*k+1. To find the
// parent of an node, go to index floor(k/2).
//
// Inserting an element involves placing it at the end of the array, then
// calling the swim() function to repeatedly compare the new element against
// that of its parent, and exchanging elements if they are not in proper order.
//
// This implementation support both MAX and MIN oriented priority queues by
// accepting a type parameter when initializing the priority queue.
//
// Removing the MIN or MAX element involves exchanging the last and first 
// elements in the tree, returning the previous first element, then repeatedly 
// calling the sink function on the new first element, comparing it against both 
// of its children and if the element is smaller, exchanging it with the larger 
// of its  two children, repeating this process until the elements are in proper 
// order.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "priority_queue.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Private function
bool pq_less(pq_t *pq, void *a, void *b);

// Generic priority_queue data type.
typedef struct pq {
  size_t max;                     // maximum size of pq
  unsigned int n;                 // number of items in pq
  size_t item_size;               // size of each item in pq
  bool (*less)(void *, void *);   // comparison function pointer
  void *items;                    // array of items
  enum pq_type type;              // MIN or MAX
} pq_t;

// Helper functions
void exchange(pq_t *pq, int i, int j);
void sink(pq_t *pq, int idx);
void swim(pq_t *pq, int idx);
void pq_print(pq_t *pq);

// Creates an empty priority queue that holds n elements of a given
// item_size, and uses the less function to compare them.
// Returns a pointer to the priority queue. Must call pq_free() when finished 
// using the priority queue.
pq_t *pq_init(size_t item_size, size_t max, enum pq_type type, bool (*less)(void *, void *)) {
  pq_t *pq = malloc(sizeof(pq_t));
  if (!pq) {
    printf("Failed to malloc pq\n");
    exit(1);
  }

  pq->max = max;
  pq->n = 0;
  pq->less = less;
  pq->item_size = item_size;
  pq->items = malloc(item_size * (max + 1));  // +1 because we do not use idx 0
  if (!pq->items) {
    printf("Failed to malloc pq->items\n");
    exit(1);
  }

  pq->type = type;

  return pq;
}


// Moves the item at the given idx upward in the binary heap to its
// heap-ordered position, where it is smaller than or equal to its parent.
void swim(pq_t *pq, int idx) {
  int parent_idx = idx / 2;
  while (idx > 1 && pq_less(pq, pq->items + parent_idx * pq->item_size, pq->items + idx * pq->item_size)) {
    exchange(pq, idx, parent_idx);
    idx = parent_idx;
    parent_idx = idx / 2;
  }
}

// Moves the item at the given idx downward in the binary heap to
// its heap-ordered position, where it is larger than or equal to both its 
// children.
void sink(pq_t *pq, int idx) {
  int child_idx1 = idx * 2;
  int child_idx2 = idx * 2 + 1;
  
  while (idx * 2 <= pq->n) {
    // Only one child to worry about
    if (child_idx2 > pq->n) {
      if (!pq_less(pq, pq->items + child_idx1 * pq->item_size, pq->items + idx * pq->item_size)) {
        exchange(pq, idx, child_idx1);
        idx = child_idx1;
      } else {
        break;
      }
    // Need to check against largest child
    } else {
      // child2 is larger
      if (pq_less(pq, pq->items + child_idx1 * pq->item_size, pq->items + child_idx2 * pq->item_size)) {
        // Compare against child2
        if (!pq_less(pq, pq->items + child_idx2 * pq->item_size, pq->items + idx * pq->item_size)) {
          exchange(pq, idx, child_idx2);
          idx = child_idx2;
        } else {
          break;
        }
      // child 1 is larger, so compare against it
      } else if (!pq_less(pq, pq->items + child_idx1 * pq->item_size, pq->items + idx * pq->item_size)) {
        exchange(pq, idx, child_idx1);
        idx = child_idx1;
      } else {
        break;
      }
    }
    child_idx1 = idx * 2;
    child_idx2 = idx * 2 + 1;
  }
}

// Swaps element at index i with element at index j
void exchange(pq_t *pq, int i, int j) {
  // store arr[i] in tmp
  uint8_t tmp[pq->item_size];
  memcpy(tmp, pq->items + pq->item_size * i, pq->item_size);

  // arr[i] = arr[j]
  memcpy(pq->items + pq->item_size * i, pq->items + pq->item_size * j, pq->item_size);

  // arr[j] = tmp
  memcpy(pq->items + pq->item_size * j, tmp, pq->item_size);
}

// Inserts an item into the priority queue. Adds it to the end of the array,
// then swims up the binary heap to its proper position.
bool pq_insert(pq_t *pq, void *item) {
  if (pq->n == pq->max) {
    return false;
  }
  // Increase number of elements
  pq->n++;
  // Copy data to last index and swim up
  memcpy(pq->items + pq->n * pq->item_size, item, pq->item_size);
  swim(pq, pq->n);

  return true;
}

// Finds the item with the max value from the priority queue and copies it to
// the memory address of item.
// Returns true if successful, false otherwise.
bool pq_front(pq_t *pq, void *item) {
  if (pq_is_empty(pq)) {
    return false;
  }
  // Return element at index 1
  memcpy(item, pq->items + pq->item_size, pq->item_size);
  return false;
}

// Removes the item with the max value from the priority queue and copies it to
// the memory address of item.
// Returns true if successful, false otherwise.
bool pq_del(pq_t *pq, void *item) {
  if (pq_is_empty(pq)) {
    return false;
  }
  // Copy the first element
  memcpy(item, pq->items + pq->item_size, pq->item_size);

  // Swap the first and last elements
  exchange(pq, 1, pq->n);

  // Decrement the last element
  pq->n--;

  // Sink the new first element
  sink(pq, 1);

  return true;
}

// Returns true if the priority queue is empty, false otherwise.
bool pq_is_empty(pq_t *pq) {
  return pq->n == 0;
}

// Returns the number of items in the priority queue.
size_t pq_size(pq_t *pq) {
  return pq->n;
}

// Frees memory associated with the priority queue.
void pq_free(pq_t *pq) {
  free(pq->items);
  free(pq);
  return;
}

// Prints current contents of priority queue. Used for debug only, and works
// only for a priority queue of characters.
void pq_print(pq_t *pq) {
  for (int i = 0; i < pq->max; i++) {
    printf("%c ", *(char *)(pq->items + i * pq->item_size));
  }
  printf("\n");
}

// Wrapper function that returns the proper result for the "less" function
// depending on if this is a MIN or MAX priority queue.
bool pq_less(pq_t *pq, void *a, void *b) {
  bool result = pq->less(a, b);

  if (pq->type == MAX) {
    return result;
  } else {
    return !result;
  }
}