// Implementation for a generic sort function. sort() is a wrapper for a variety
// of different implementations that can be selected via #define in the header.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "sort.h"

void exchange(void *arr, size_t item_size, int i, int j);

void selectionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void insertionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void shellsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

// my_ prefix to distinguish from C stdlib implementations
void my_mergesort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void my_heapsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void my_quicksort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

void sort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  shellsort(arr, item_size, n, less);
}

bool is_sorted(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  for (int i = 1; i < n; i++) {
    if (less(arr + item_size * i, arr + item_size * (i - 1))) {
      return false;
    }
  }
  return true;
}

// Starting at the first element, finds the smallest remaining element in the 
// array and exchanges it with the first element. Then starting at the second
// element, finds the smallest remaining element in the array and exchanges it
// with the second element. And so forth.
void selectionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  for (int i = 0; i < n; i++) {
    int min_idx = i;
    for (int j = i + 1; j < n; j++) {
      if (less(arr + j * item_size, arr + min_idx * item_size)) {
        min_idx = j;
      }
    }
    exchange(arr, item_size, i, min_idx);
  }
}

// Starting with the second element, compares the current element against each 
// preceding element, exchanging positions with it if the current element is
// smaller than that preceding element. This eventually shifts the element to
// its correct sorted position. Then move to the third element, and repeat the
// backward comparison/exchanges. Continue until the last element in the array.
void insertionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  for (int i = 1; i < n; i++) {
    int j = i;
    while (j > 0 && less(arr + j * item_size, arr + (j - 1) * item_size)) {
      exchange(arr, item_size, j, j - 1);
      j--;
    }
  }
}

// Same as insertion sort, but starts by considering elements that are a stride
// "h" distance apart in the array. The value of h continually decreases until
// it reaches 1, at which point the algorithm is exactly the same as insertion
// sort. The Knuth sequence of h = 3h + 1 is used.
void shellsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  int h;

  // Compute h as the largest in the Knuth sequence of 3*h + 1 that is smaller
  // than n
  h = 1;
  while (h < n / 3) {
    h = 3 * h + 1;
  }

  // Insertion sort algorithm with step size of h
  while (h >= 1) {
    for (int i = h; i < n; i++) {
      int j = i;
      while (j >= h && less(arr + j * item_size, arr + (j - h) * item_size)) {
        exchange(arr, item_size, j, j - h);
        j -= h;
      }
    }
    h /= 3;
  }
}

// Swaps element at index i with element at index j
void exchange(void *arr, size_t item_size, int i, int j) {
  // store arr[i] in tmp
  uint8_t tmp[item_size];
  memcpy(tmp, arr + item_size * i, item_size);

  // arr[i] = arr[j]
  memcpy(arr + item_size * i, arr + item_size * j, item_size);

  // arr[j] = tmp
  memcpy(arr + item_size * j, tmp, item_size);
}

// Comparison functions
bool less_int(void *v, void *w) {
  return *(int *)v < *(int *)w;
}

bool less_uint(void *v, void *w) {
  return *(unsigned int *)v < *(unsigned int *)w;
}

bool less_float(void *v, void *w) {
  return *(float *)v < *(float *)w;
}

bool less_double(void *v, void *w) {
  return *(double *)v < *(double *)w;
}

bool less_char(void *v, void *w) {
  return *(char *)v < *(char *)w;
}

bool less_str(void *v, void *w) {
  return (strcmp(*(char **)v, *(char **)w) < 0);
}