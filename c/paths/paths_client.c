// Implements a degrees-of-separation client by building a symbol graph from a
// given input file and using graph search to find the path between vertices in
// the graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "symbol_graph.h"
#include "../paths/paths.h"
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Not enough arguments\nUsage: ./paths_client <filename> <delimiter> <source_key>");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  char *delimiter = argv[2];
  char *source_key = argv[3];
  printf("Generating symbol table for file %s...\n", filename);
  sg_t *sg = sg_init(filename, delimiter);
  printf("%d keys found\n", sg_size(sg));
  
  if (!sg_contains(sg, source_key)) {
    printf("%s not found\n", source_key);
    return 0;
  }
  int source_index = sg_index_of(sg, source_key);

  printf("Generating all paths from %s...\n", source_key);
  paths_t *p = paths_init(sg_graph(sg), source_index, BFS);

  char input[MAX_LINE_LENGTH];
  char *query;
  int query_index;

  printf("\nEnter a performer's name <Last Name, First Name> or a film name <Name (year)>:\n");
  while (fgets(input, MAX_LINE_LENGTH, stdin) != NULL) {
    input[strcspn(input, "\n")] = 0;  // replace \n with null-terminator
    query = strdup(input);

    if (!sg_contains(sg, query)) {
      printf("%s is not in the database!\n", query);
      continue;
    }

    query_index = sg_index_of(sg, query);

    if (!has_path_to(p, query_index)) {
      printf("There is no path between %s and %s!\n", source_key, query);
      continue;
    }

    int next_index;
    path_to_iter_init(p, query_index);
    while (path_to_iter_has_next(p, query_index)) {
      path_to_iter_next(p, query_index, &next_index);
      char *next_key = sg_name_of(sg, next_index);
      printf("\t%s\n", next_key);
    }
  }

  paths_free(p);

  return 0;  
}
