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

void mergesort_td(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void merge(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int mid, int hi, void *aux);
void mergesort_td_recursive(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi, void *aux);

void mergesort_bu(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

void my_heapsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void my_quicksort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

void sort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  mergesort_bu(arr, item_size, n, less);
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

// Entry to top-down mergesort. Sets up a parallel aux array for the merge 
// operation and calls the recursive mergesort_td_recursive function that 
// actually performs the sort.
void mergesort_td(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  void *aux = malloc(n * item_size);
  if (!aux) {
    perror("Failed to malloc");
    exit(EXIT_FAILURE);
  }

  mergesort_td_recursive(arr, item_size, less, 0, n - 1, aux);
  free(aux);
}

// Top-down mergesort recursively splits the array to be sorted in half, sorts 
// each half, then merges the sorted halves together using the merge function.
void mergesort_td_recursive(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi, void *aux) {
  if (hi <= lo) {
    return;
  }
  int mid = lo + (hi - lo) / 2;

  // Left half
  mergesort_td_recursive(arr, item_size, less, lo, mid, aux);

  // Right half
  mergesort_td_recursive(arr, item_size, less, mid + 1, hi, aux);

  // Merge
  merge(arr, item_size, less, lo, mid, hi, aux);
}

// Bottom-up mergesort uses the merge function to merge progressively larger
// pairs of arrays, starting with each 1x1 pair of elements in the array, then
// each pair of 2x2 elements in the array, and so forth. Like top-down
// mergesort, this function creates an auxiliary array of size n that is used
// during the merge.
void mergesort_bu(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  void *aux = malloc(n * item_size);
  if (!aux) {
    perror("Failed to malloc");
    exit(EXIT_FAILURE);
  }

  // len is the length of each half that will then be merged
  int mid, hi;
  for (int len = 1; len < n; len *= 2) {
    for (int lo = 0; lo < n - len; lo += (len + len)) {
      mid = lo + len - 1;
      hi = lo + len + len - 1;
      // special case to ensure the last merge does not go out of bounds
      if (hi > n - 1) {
        hi = n - 1;
      }
      merge(arr, item_size, less, lo, mid, hi, aux);
    }
  }
  free(aux);
}

// Merges two sorted halves of array, arr[lo:mid] and arr[mid+1:hi]. Uses 
// auxiliary memory proportional to the number of elements.
void merge(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int mid, int hi, void *aux) {
  int i = lo;       // index into the left half
  int j = mid + 1;  // index into the right half

  // Copy over the array to aux
  for (int k = lo; k <= hi; k++) {
    memcpy(aux + k * item_size, arr + k * item_size, item_size);
  }

  // Perform the merge
  for (int k = lo; k <= hi; k++) {
    // Left half exhausted, take from the right
    if (i > mid) {
      memcpy(arr + k * item_size, aux + j * item_size, item_size);
      j++;
    // Right half exhaused, take from the left
    } else if (j > hi) {
      memcpy(arr + k * item_size, aux + i * item_size, item_size);
      i++;
    // Right less than left, take from right
    } else if (less(aux + j * item_size, aux + i * item_size)) {    // compare the aux values, because the arr values are changing
      memcpy(arr + k * item_size, aux + j * item_size, item_size);
      j++;
    // Right greater than or equal to left, take from left
    } else {
      memcpy(arr + k * item_size, aux + i * item_size, item_size);
      i++;
    }
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