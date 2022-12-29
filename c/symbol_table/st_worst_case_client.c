// Symbol table client that generates a worst-case BST with integer keys 
// inserted in numerical order. For an unbalanced BST, this results in a tree of
// depth N. For a red-black BST this results in a tree of depth 2 * lg N.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "symbol_table.h"

int main(int argc, char *argv[]) {
  int st_types[] = {BST, RED_BLACK_BST, SEQUENTIAL_SEARCH, HASH_TABLE_CHAINING, HASH_TABLE_PROBING};
  char *st_names[] = {"BST", "RED_BLACK_BST", "SEQUENTIAL_SEARCH", "HASH_TABLE_CHAINING", "HASH_TABLE_PROBING"};

  if (argc < 3) {
    printf("Missing arguments\nUsage: st_worst_case_client <st_type> <count>\n");
    exit(EXIT_FAILURE);
  }
  int i = atoi(argv[1]);
  int total = atoi(argv[2]);

  st_t *st = st_init(sizeof(int), sizeof(int), compare_int, st_types[i]);

  int value = 0;
  clock_t t;
  clock_t t_get = 0;
  clock_t t_put = 0;
  for (int j = 0; j < total; j++) {    
    t = clock();
    st_get(st, &j, &value);
    t_get += (clock() - t);
    
    t = clock();
    st_put(st, &j, &value);
    t_put += (clock() - t);
  }

  printf("%s: get (cum sec): %f, put (cum sec): %f\n", st_names[i], (double)t_get / CLOCKS_PER_SEC, (double)t_put / CLOCKS_PER_SEC);

  int unique = st_size(st);
  st_iter_init(st);
  
  int key;
  t_get = 0;
  for (int j = 0; j < total; j++) {    
    st_iter_next(st, &key);

    t = clock();
    st_get(st, &key, &value);
    t_get += (clock() - t);
  }

  printf("%s: get (cum sec): %f\n", st_names[i], (double)t_get / CLOCKS_PER_SEC);
  printf("Unique: %d\n", unique);

  free(st);

  return 0;
}