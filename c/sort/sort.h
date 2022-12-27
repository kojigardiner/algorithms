// Interface for a generic sort function.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "../lib/lib.h"

enum sort_type {
  SELECTION,
  INSERTION,
  SHELL,
  MERGE_TD,
  MERGE_BU,
  QUICK,
  HEAP,
  STR_LSD,
  STR_MSD,
  STR_THREEWAY_QUICK,
};

// Sorts an array of generic items of the given item_size, using a pointer to a
// comparison function less.
void sort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *), enum sort_type type);

// Returns true if an array is in sorted order, according to the comparsion
// function less. Returns false otherwise.
bool is_sorted(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

// Randomly shuffles an array in place. Note that for small array sizes this is
// not precisely uniform.
void shuffle(void *arr, size_t item_size, size_t n);
