// Symbol table client that reads input text and computes the frequency of 
// occurrence of individual words.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

int main(int argc, char *argv[]) {
  FILE *fp;
  if (argc > 1) {
    fp = fopen(argv[1], "r");
    if (!fp) {
      perror("Failed to open file\n");
      exit(EXIT_FAILURE);
    }
  } else {
    fp = stdin;
  }

  st_t *st = st_init(sizeof(char *), sizeof(int), compare_str);

  // Read inputs
  char read_str[256];
  char *store_str;
  int count;
  int total = 0;
  while (fscanf(fp, "%255s", read_str) > 0) {
    store_str = strdup(read_str);
    count = 0;
    st_get(st, &store_str, &count);
    count++;
    st_put(st, &store_str, &count);
    total++;
  }

  // Print totals
  char *key;
  char *max_key = NULL;
  int max_value = 0;
  int value;
  int unique = st_size(st);
  st_iter_init(st);
  while (st_iter_has_next(st)) {
    st_iter_next(st, &key);
    st_get(st, &key, &value);
    if (value > max_value) {
      max_value = value;
      if (max_key) {
        free(max_key);
      }
      max_key = strdup(key);
    }
  }
  printf("Words: %d\n", total);
  printf("Unique: %d\n", unique);
  printf("Most frequent: %s\nCount: %d\n", max_key, max_value);

  return 0;
}