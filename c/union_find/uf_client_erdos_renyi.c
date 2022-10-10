// Union-find client that accepts one command-line argument, a count n, and
// prints the number of iterations of random sites pairs required to fully
// connect all n sites. Per the Erdos-Renyi model, this should scale as
// ~ 1/2 n * ln (n).
#include "uf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int count(int n) {
  int p, q, connections;
  uf_t *uf = uf_init(n);
  srand(time(NULL));   // seed PRNG

  connections = 0;

  while (uf_count(uf) > 1) {
    p = rand() % n;
    q = rand() % n;
    //printf("%d %d\n", p, q);

    if (!uf_connected(uf, p, q)) {
      uf_union(uf, p, q);
    }
    connections++;
  }

  uf_free(uf);
  return connections;
}

int main(int argc, char *argv[]) {
  FILE *fp;
  int n;

  if (argc > 1) {
    n = atoi(argv[1]);
    printf("Fully connnecting %d sites required %d connections vs %f expected from Erdos-Renyi\n", n, count(n), 0.5 * n * log(n));
  } else {
    printf("Error: provide a count n\n");
    return 1;
  }

  return 0;
}