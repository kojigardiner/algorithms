// Key-word in context (KWIC) client, which makes use of suffix arrays and
// string sorting.

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include "sort.h"

// Stores start/end indices for suffix array
struct indices {
  int start;
  int end;
};

// Prints n characters from string str, replacing \n and \r with spaces
void print_chars(char *str, int n) {
  for (int i = 0; i < n; i++) {
    char c = str[i];
    if (c != '\n' && c != '\r') {
      printf("%c", c);
    } else {
      printf(" ");
    }
  }
  printf("\n");
}

// Uses binary search to find the indices of the first and last occurrence of
// the given search string in an array of strings. Returns true if the search
// string is found, false otherwise. Start/end indices are updated in the
// indices struct.
bool index_of_substring(char **suffixes, int n, char *search, struct indices *indices) {
  int lo = 0;
  int hi = n;
  int mid;

  while (lo <= hi) {
    mid = (hi - lo) / 2 + lo;

    // Check to make sure the current suffix isn't too short
    // if (strlen(suffixes[mid]) < strlen(search)) {
    //   break;
    // }

    // Compare as many characters as are in the search string
    int cmp = strncmp(search, suffixes[mid], strlen(search));
    if (cmp < 0) {
      hi = mid - 1;
    } else if (cmp > 0) {
      lo = mid + 1;
    } else {
      // Found one match, so check indices on either side until there is no
      // longer a match
      int start = mid;
      int end = mid;
      while (start > 0 && strncmp(search, suffixes[start - 1], strlen(search)) == 0) {
        start--;
      }
      while (end < n && strncmp(search, suffixes[end + 1], strlen(search)) == 0) {
        end++;
      }
      indices->start = start;
      indices->end = end;
      return true;
    }
  }

  return false;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: kwic_client <filename> <num_context_chars>\n");
    exit(EXIT_FAILURE);
  }
  struct stat st;

  char **suffixes;
  char *buffer;
  char *filename = argv[1];
  int num_chars = atoi(argv[2]);

  // Open file and read its full contents into memory
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    printf("Failed to open %s\n", filename);
    exit(EXIT_FAILURE);
  }
  stat(filename, &st);
  buffer = malloc(st.st_size);
  if (!buffer) {
    fclose(fp);
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  int nbytes = fread(buffer, 1, st.st_size, fp);
  fclose(fp);
  if (nbytes != st.st_size) {
    printf("Failed to read entire file\n");
    exit(EXIT_FAILURE);
  }
  
  // Create an array of char * pointers and set each to point to each suffix
  suffixes = malloc(sizeof(char *) * nbytes);
  if (!suffixes) {
    fclose(fp);
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < nbytes; i++) {
    suffixes[i] = &buffer[i];
  }

  // Sort the suffix array of string pointers
  sort(suffixes, sizeof(char *), nbytes, less_str, STR_THREEWAY_QUICK);

  // User query loop
  char input[512];
  struct indices indices;
  while (1) {
    printf("\nEnter search string: ");
    fgets(input, 512, stdin);
    // remove the newline character
    input[strlen(input) - 1] = '\0';

    // If the search substring is found, print out all matches
    if (index_of_substring(suffixes, nbytes, input, &indices)) {
      printf("'%s' found!\n", input);
      for (int i = indices.start; i <= indices.end; i++) {
        print_chars(suffixes[i], strlen(input) + num_chars);
      }
    } else {
      printf("'%s' not found!\n", input);
    }
  }

  free(buffer);
  free(suffixes);

  return 0;
}