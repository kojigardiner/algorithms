// Test client for generic bag data type
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bag.h"

int main() {
  // Test a bag of ints
  int value;
  int values[] = {10, 3, 4, -11};
  bag_t *b = bag_init(sizeof(int));

  assert(bag_is_empty(b));

  for (int i = 0; i < NELEMS(values); i++) {
    value = values[i];
    bag_add(b, &value);
    printf("Added %d to bag\n", value);
  }

  assert(bag_size(b) == 4);

  bag_iter_init(b);
  while (bag_iter_has_next(b)) {
    bag_iter_next(b, &value);
    printf("Iterated %d in bag\n", value);
  }

  bag_free(b);

  // Test a bag of strings (stored on the heap)
  char *str;
  char *strs[] = {"hello", "world", "this is me", "what"};;
  b = bag_init(sizeof(char *));

  assert(bag_is_empty(b));

  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    bag_add(b, &str);
    printf("Added %s to bag\n", str);
  }
  
  assert(bag_size(b) == 4);
  
  bag_iter_init(b);
  while (bag_iter_has_next(b)) {
    bag_iter_next(b, &str);
    printf("Iterated %s in bag\n", str);
  }

  bag_free(b);

  printf("Passed all tests.\n");

  return 0;
}