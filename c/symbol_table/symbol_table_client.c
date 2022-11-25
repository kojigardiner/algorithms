// Symbol table client that reads input text and computes the frequency of 
// occurrence of individual words.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "symbol_table.h"

int main(int argc, char *argv[]) {
  int st_types[] = {BST, RED_BLACK_BST, SEQUENTIAL_SEARCH, HASH_TABLE};
  char *st_names[] = {"BST", "RED_BLACK_BST", "SEQUENTIAL_SEARCH", "HASH_TABLE"};

  FILE *fp;
  if (argc < 2) {
    printf("Missing arguments\nUsage: symbol_table_client st_type (filename)\n");
    exit(EXIT_FAILURE);
  }
  int i = atoi(argv[1]);
  if (argc > 2) {
    fp = fopen(argv[2], "r");
    if (!fp) {
      perror("Failed to open file\n");
      exit(EXIT_FAILURE);
    }
  } else {
    fp = stdin;
  }

  st_t *st = st_init(sizeof(char *), sizeof(int), compare_str, st_types[i]);

  // Read inputs
  char read_str[256];
  char *store_str;
  int count;
  int total = 0;
  clock_t t;
  clock_t t_get = 0;
  clock_t t_put = 0;
  while (fscanf(fp, "%255s", read_str) > 0) {
    store_str = strdup(read_str);
    count = 0;
    
    t = clock();
    st_get(st, &store_str, &count);
    t_get += (clock() - t);
    count++;
    
    t = clock();
    st_put(st, &store_str, &count);
    t_put += (clock() - t);
    
    total++;
  }

  printf("%s: get (cum sec): %f, put (cum sec): %f\n", st_names[i], (double)t_get / CLOCKS_PER_SEC, (double)t_put / CLOCKS_PER_SEC);

  // Print totals
  char *key;
  char *max_key = NULL;
  int max_value = 0;
  int value;
  int unique = st_size(st);
  st_iter_init(st);
  
  t_get = 0;
  while (st_iter_has_next(st)) {
    st_iter_next(st, &key);
    
    t = clock();
    st_get(st, &key, &value);
    t_get += (clock() - t);

    if (value > max_value) {
      max_value = value;
      if (max_key) {
        free(max_key);
      }
      max_key = strdup(key);
    }
  }
  printf("%s: get (cum sec): %f\n", st_names[i], (double)t_get / CLOCKS_PER_SEC);
  printf("Words: %d\n", total);
  printf("Unique: %d\n", unique);
  printf("Most frequent: %s\nCount: %d\n", max_key, max_value);

  free(st);

  return 0;
}