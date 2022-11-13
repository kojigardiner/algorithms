// Sort client that accepts ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "sort.h"

void init_arr(int *arr, int len) {  
  for (int i = 0; i < len; i++) {
    arr[i] = rand();
  }
}

int main(int argc, char *argv[]) {
  int len = 1000;
  int *arr = NULL;
  int *arr_cpy = NULL;
  clock_t t;
  enum sort_type types[7] = {SELECTION, INSERTION, SHELL, MERGE_TD, MERGE_BU, QUICK, HEAP};
  srand(time(NULL));

  printf("type,length,time\n");
  for (int i = 0; i < 5; i++) {
    arr = malloc(len * sizeof(int));
    arr_cpy = malloc(len * sizeof(int));
    init_arr(arr, len);

    for (int j = 0; j < NELEMS(types); j++) {      
      memcpy(arr_cpy, arr, len * sizeof(int));  // reset the array contents

      t = clock();
      sort(arr_cpy, sizeof(int), len, less_int, types[j]);
      t = clock() - t;

      printf("%d,%d,%f\n", types[j], len, ((double)t)/CLOCKS_PER_SEC);
    }
    len *= 2;

    free(arr);
    free(arr_cpy);
  }

  return 0;
}