// Implementation of a Symbol Graph.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../symbol_table/symbol_table.h"
#include "../graph/graph.h"

#define MAX_LINE_LENGTH 1024

typedef struct sg {
  st_t *st;   // item-to-index
  char **items;  // index-to-item
  graph_t *g; // graph
} sg_t;

sg_t *sg_init(char *filename, char *delimiter) {
  sg_t *sg = malloc(sizeof(sg_t));
  if (!sg) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  sg->st = st_init(sizeof(char *), sizeof(int), compare_str, HASH_TABLE);

  char line[MAX_LINE_LENGTH];

  char *item;
  char *token;

  FILE *fp = fopen(filename, "r");
  if (!fp){
    perror("Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  // Read each line from the file
  int index = 0;
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    line[strcspn(line, "\n")] = 0;  // replace \n with null-terminator

    // Read each delimited token from the line
    token = strtok(line, delimiter);
    if (!token) {
      continue;
    }
    do {
      item = strdup(token);
      if (!item) {
        perror("Failed to strdup\n");
        exit(EXIT_FAILURE);
      }
      // Check if the token is already in the st
      int dummy;
      if (!st_get(sg->st, &item, &dummy)) {
        // Add the token to the st, and increment our index
        st_put(sg->st, &item, &index);
        index++;
      }
      token = strtok(NULL, delimiter);
    } while (token != NULL);
  }
  
  fclose(fp);

  // Build the items array
  sg->items = malloc(st_size(sg->st) * sizeof(char *));
  if (!sg->items) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  char *key;
  st_iter_init(sg->st);
  while (st_iter_has_next(sg->st)) {
    st_iter_next(sg->st, &key);
    st_get(sg->st, &key, &index);
    sg->items[index] = strdup(key);
    if (!sg->items[index]) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
  }

  // Build the graph by reading the file again
  sg->g = graph_init(st_size(sg->st), UNDIRECTED);

  fp = fopen(filename, "r");
  if (!fp){
    perror("Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    line[strcspn(line, "\n")] = 0;  // replace \n with null-terminator

    // Read each delimited token from the line
    token = strtok(line, delimiter);
    if (!token) {
      continue;
    }

    item = strdup(token);
    if (!item) {
      perror("Failed to strdup\n");
      exit(EXIT_FAILURE);
    }
    int index1;
    st_get(sg->st, &item, &index1);

    token = strtok(NULL, delimiter);
    while (token != NULL) {
      item = strdup(token);
      if (!item) {
        perror("Failed to strdup\n");
        exit(EXIT_FAILURE);
      }
      int index2;
      st_get(sg->st, &item, &index2);

      graph_add_edge(sg->g, index1, index2);
      token = strtok(NULL, delimiter);
    }
  }
  
  fclose(fp);

  return sg;
}

bool sg_contains(sg_t *sg, char *key) {
  int dummy;
  return (st_get(sg->st, &key, &dummy));
}

int sg_index_of(sg_t *sg, char *key) {
  int index;
  if (!st_get(sg->st, &key, &index)) {
    index = -1;
  }
  return index;
}

char *sg_name_of(sg_t *sg, int index) {
  if (index < 0 || index > st_size(sg->st)-1) {
    return NULL;
  }
  return sg->items[index];
}

graph_t *sg_graph(sg_t *sg) {
  return sg->g;
}

int sg_size(sg_t *sg) {
  return st_size(sg->st);
}