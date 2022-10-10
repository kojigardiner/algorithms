// Union-find client that accepts one command-line argument, a filename, 
// containing the following information:
//    Number of sites
//    Two integers, separated by a space
//    Two integers, separated by a space
//    ...
// The program creates a union-find structure with the given number of
// sites, calls uf_union on each pair of component ids provided in the
// file, then prints the total number of connected components.
#include "uf.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  int n;
  int p, q;
  uf_t *uf = NULL;

  if (argc > 1) {
    char line[1024];
    int count = 0;
    fp = fopen(argv[1], "r");
    
    fgets(line, 1024, fp);
    sscanf(line, "%d", &n);
    uf = uf_init(n);

    while (fgets(line, 1024, fp) && count < n) {
      sscanf(line, "%d %d", &p, &q);
      uf_union(uf, p, q);  
      //printf("%d %d\n", p, q);
      count++;
    }
    printf("%d components\n", uf_count(uf));
  } else {
    printf("Error: provide a filename\n");
    return 1;
  }

  uf_free(uf);

  return 0;
}