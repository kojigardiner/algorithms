// Implements a multiway merge that can take multiple sorted files with a single
// character on each line and print the merged, sorted characters.

#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

// Struct for storing a character and a file index
typedef struct char_index {
  char c;
  int i;
} char_index_t;

// Allow use of the MAX priority queue but removing the min item instead.
bool greater_char_index(void *v, void *w) {
  return !less_char(&(*(char_index_t *)v).c, &(*(char_index_t *)w).c);
}

// Reads multiple sorted files as input, where each file consists of one character
// per line. Uses a priority queue to perform the sort in streaming fashion rather
// than reading all contents and sorting. Prints the sorted contents to stdout. 
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("No arguments provided\nUsage: priority_queue_client [filename1] ...\n");
    exit(1);
  }
  int num_files = argc - 1;

  pq_t *pq = pq_init(sizeof(char_index_t), num_files, MAX, greater_char_index);
  FILE *fp[num_files];
  char c;
  
  // Insert one char per file into the priority queue
  for (int i = 1; i < argc; i++) {
    fp[i - 1] = fopen(argv[i], "r");
    if (!fp[i - 1]) {
      perror("Could not open file\n");
      exit(1);
    }
    if (fscanf(fp[i - 1], "%c\n", &c) > 0) {
      char_index_t ci;
      ci.c = c;
      ci.i = i - 1;  // file # index

      pq_insert(pq, &ci);
    }
  }

  // Remove items in order
  while (!pq_is_empty(pq)) {
    char_index_t ci;
    pq_del(pq, &ci);
    printf("%c\n", ci.c);
    int idx = ci.i;
    
    // After we move an item from a particular file, insert the next item from
    // that file
    if (fscanf(fp[idx], "%c\n", &c) > 0) {
      ci.c = c;
      ci.i = idx;

      pq_insert(pq, &ci);
    }
  }

  for (int i = 0; i < num_files; i++) {
    fclose(fp[i]);
  }

  pq_free(pq);

  return 0;
}
