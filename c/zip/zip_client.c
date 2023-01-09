// Test client for zipping files.

#include "zip.h"
#include "../lib/lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

static const char *filenames[] = {"../data/abra.txt", "../data/abraLZW.txt", "../data/ababLZW.txt", "../data/4runs.bin", "../data/q64x96.bin", "../data/tinytinyTale.txt", "../data/tinyTale.txt", "../data/medTale.txt", "../data/tale.txt"};
static const char *compressed_filename = "compressed";
static const char *expanded_filename = "expanded";

static int types[] = {RLE, HUFFMAN, LZW};
static char *type_names[] = {"RLE", "HUFFMAN", "LZW"};

int main(int argc, char **argv) {
  clock_t t;
  double ratio;
  for (int i = 0; i < NELEMS(filenames); i++) {
    printf("%s\n", filenames[i]);
    for (int j = 0; j < NELEMS(types); j++) {
      t = clock();
      ratio = compress(filenames[i], compressed_filename, types[j]);
      printf("%15s %10s %f sec %.1f%%\n", type_names[j], "compress:", (double)(clock() - t) / CLOCKS_PER_SEC, ratio * 100);

      t = clock();
      expand(compressed_filename, expanded_filename, types[j]);
      printf("%15s %10s %f sec\n", type_names[j], "expand:", (double)(clock() - t) / CLOCKS_PER_SEC);

      if (verify(filenames[i], expanded_filename)) {
        printf("%15s\n", "Verified!");
      } else {
        printf("%15s\n", "Not Verified!");
      }
    }
  }
}