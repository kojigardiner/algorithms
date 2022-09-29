// Test client for generic queue data type
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

int main() {
  // Test a queue of ints
  printf("\nTesting queue implementation with ints\n");
  int value;
  int values[] = {10, 3, 4, -11};
  queue_t *q = queue_init(sizeof(int));

  assert(queue_is_empty(q));

  for (int i = 0; i < NELEMS(values); i++) {
    value = values[i];
    queue_enqueue(q, &value);
    printf("+ Enqueued %d\n", value);
  }

  assert(queue_size(q) == 4);

  queue_iter_init(q);
  while (queue_iter_has_next(q)) {
    queue_iter_next(q, &value);
    printf("o Iterated %d\n", value);
  }

  while (!queue_is_empty(q)) {
    queue_dequeue(q, &value);
    printf("- Dequeued %d\n", value);
  }

  assert(queue_is_empty(q));

  queue_free(q);

  // Test a queue of strings (stored on the heap)
  printf("\nTesting queue implementation with strings\n");
  char *str;
  char *strs[] = {"hello", "world", "this is me", "what"};;
  q = queue_init(sizeof(char *));

  assert(queue_is_empty(q));

  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    queue_enqueue(q, &str);
    printf("+ Enqueued %s\n", str);
  }
  
  assert(queue_size(q) == 4);
  
  queue_iter_init(q);
  while (queue_iter_has_next(q)) {
    queue_iter_next(q, &str);
    printf("o Iterated %s\n", str);
  }

  while (!queue_is_empty(q)) {
    queue_dequeue(q, &str);
    printf("- Dequeued %s\n", str);
  }

  assert(queue_is_empty(q));

  queue_free(q);

  printf("\nPassed all tests\n");

  return 0;
}