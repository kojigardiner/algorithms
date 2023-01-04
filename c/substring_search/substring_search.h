// Interface for substring search.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include "../lib/lib.h"

#define RADIX 256

enum search_type {
  BRUTE,
  KMP, // Knuth-Morris-Pratt
  BOYER_MOORE,
  RABIN_KARP,
};

// Search for the given pattern in the given text. Returns the starting position
// of the pattern if found, or -1 otherwise.
int substring_search(char *pattern, char *text, enum search_type type);
