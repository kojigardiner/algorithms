// Test client for substring search on a large body of text.

#include "substring_search.h"
#include "../lib/lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./substring_search_client <filename>\n");
    exit(EXIT_FAILURE);
  }

  // Read the input file into a buffer
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    printf("Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  struct stat st;
  stat(argv[1], &st);

  char *buffer = malloc(st.st_size);
  if (!buffer) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  int nbytes = fread(buffer, 1, st.st_size, fp);
  if (nbytes != st.st_size) {
    printf("Failed to read complete file\n");
    exit(EXIT_FAILURE);
  }

  int types[] = {BRUTE, KMP, BOYER_MOORE, RABIN_KARP};
  char *type_names[] = {"BRUTE", "KMP", "BOYER_MOORE", "RABIN_KARP"};
  
  char input[1024];
  while (1) {
    printf("Enter search string: ");
    fgets(input, 1024, stdin);
    // remove newline
    input[strlen(input) - 1] = '\0';

    for (int i = 0; i < NELEMS(types); i++) {
      clock_t t = clock();
      int position = substring_search(input, buffer, types[i]);
      printf("%15s: idx %d, %f sec\n", type_names[i], position, (double)(clock() - t) / CLOCKS_PER_SEC);
    }
  }

  free(buffer);
}