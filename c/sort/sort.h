// Interface for a generic sort function.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

enum sort_type {
  SELECTION,
  INSERTION,
  SHELL,
  MERGE_TD,
  MERGE_BU,
  QUICK,
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

// Comparison functions for commmon types. Comparison functions for more
// complex types must be provided by the client.
bool less_int(void *v, void *w);
bool less_uint(void *v, void *w);
bool less_float(void *v, void *w);
bool less_double(void *v, void *w);
bool less_char(void *v, void *w);
bool less_str(void *v, void *w);