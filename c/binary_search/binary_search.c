#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binary_search.h"

// Uses binary search to find the index of a key in a sorted array a
int index_of(int *a, size_t len, int key) {
  // Array must be sorted
  int lo = 0;
  int hi = len - 1;
  int mid;

  while (lo <= hi) {  // Key is in a[lo..hi] or not present
    mid = lo + (hi - lo) / 2;
    if (key < a[mid]) {
      hi = mid - 1;
    } else if (key > a [mid]) {
      lo = mid + 1;
    } else {
      return mid;
    }
  }
  return -1;
}

// Compares integers
int cmp_int(const void *a, const void *b) {
  int c = *((int *)a);
  int d = *((int *)b);

  if (c > d) {
    return 1;
  }
  if (c < d) {
    return -1;
  }
  
  return 0;
}

// Unit tests
int main(int argc, char **argv) {
  // Small array
  int in[] = {55, 1, 4, 70, 9, 6, 33, 100};
  //  sorted {1, 4, 6, 9, 33, 55, 70, 100}
  qsort(in, NELEMS(in), sizeof(in[0]), cmp_int);  
  assert(index_of(in, NELEMS(in), 1) == 0);     // First element
  assert(index_of(in, NELEMS(in), 100) == 7);   // Last element
  assert(index_of(in, NELEMS(in), 20) == -1);   // Missing element

  printf("All tests passed\n");

  // Newline-delimited text of integers
  if (argc > 1) {
    printf("Running whitelist test\n");
    int *in_list;
    int curr;
    int count = 0;

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
      printf("Could not open input file\n");
      return 1;
    }
    while(fscanf(input_file, "%d\n", &curr) > 0) {
      count++;
      in_list = (int *)realloc(in_list, count * sizeof(*in_list));
      if (!in_list) {
        printf("Could not realloc in_list\n");
        return 1;
      }
      in_list[count - 1] = curr;
    }
    qsort(in_list, count, sizeof(in_list[0]), cmp_int);
    while (fscanf(stdin, "%d", &curr) > 0) {
      if (index_of(in_list, count, curr) == -1) {
        printf("%d not in whitelist\n", curr);
      }
    }
  }

  return 0;
}