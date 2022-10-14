// Sort client that accepts ...
#include <stdio.h>
#include <stdbool.h>
#include "sort.h"

int main(int argc, char *argv[]) {
  int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  if (is_sorted(arr, sizeof(arr[0]), 10, less_int)) {
    printf("Sorted!\n");
  } else {
    printf("Not sorted\n");
  }

  return 0;
}