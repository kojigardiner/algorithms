// Client for testing the "keys with prefix" iterator for the trie symbol
// table.

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

#define MAX_STR_LENGTH 1024

void str_tolower(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = tolower(str[i]);
  }
}

int main(int argc, char **argv) {
  FILE *fp = fopen("../data/dictionary.txt", "r");
  if (!fp) {
    printf("Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  st_t *st = st_init(sizeof(char *), sizeof(char), compare_str, TRIE_RWAY);

  char str[MAX_STR_LENGTH];
  char *stralloc;

  // Read from dictionary file
  char value = 'a';
  while (fscanf(fp, "%s\n", str) == 1) {
    str_tolower(str);
    // printf("%s\n", str);

    stralloc = strdup(str);
    st_put(st, &stralloc, &value);
  }

  printf("Type a word or partial word and hit enter to see all words in the dictionary that start with those characters:\n");
  while (1) {
    printf(">> ");
    fgets(str, MAX_STR_LENGTH, stdin);
    str[strlen(str) - 1] = '\0';  // remove newline
    st_prefix_iter_init(st, str);
    while (st_prefix_iter_has_next(st)) {
      st_prefix_iter_next(st, &stralloc);
      printf("   %s\n", stralloc);
    }
  }

  return 0;
}