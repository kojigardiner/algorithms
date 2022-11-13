// Implementation for a generic sort function. sort() is a wrapper for a variety
// of different implementations that can be selected via #define in the header.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sort.h"

// Sort functions
void selectionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void insertionsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void shellsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void mergesort_td(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void mergesort_bu(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
// my_ prefix to distinguish from C stdlib implementations
void my_quicksort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));
void my_heapsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *));

// Helper functions
void exchange(void *arr, size_t item_size, int i, int j);
void merge(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int mid, int hi, void *aux);
void mergesort_td_recursive(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi, void *aux);
int partition(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi);
void my_quicksort_recursive(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi);
void sink(void *arr, size_t item_size, int idx, size_t n, bool (*less)(void *, void *));

void sort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *), enum sort_type type) {
  switch (type) {
    case SELECTION:
      selectionsort(arr, item_size, n, less);
      break;
    case INSERTION:
      insertionsort(arr, item_size, n, less);
      break;
    case SHELL:
      shellsort(arr, item_size, n, less);
      break;
    case MERGE_TD:
      mergesort_td(arr, item_size, n, less);;
      break;
    case MERGE_BU:
      mergesort_bu(arr, item_size, n, less);
      break;
    case QUICK:
      my_quicksort(arr, item_size, n, less);
      break;
    case HEAP:
      my_heapsort(arr, item_size, n, less);
      break;
    default:
      perror("Unexpected sort type");
      exit(EXIT_FAILURE);
  }
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

// Performs quicksort by recursively partitioning and sorting sub-arrays by
// random partition elements. The array is first randomly shuffled, then the
// partition is selected as the first element in the array. Starting at each end
// of the sub-array, elements are then compared against the partitioning element
// and swapped if they are found to be out-of-order. Note that this is a naive
// version of quicksort that does not perform well for arrays with numerous
// duplicate entries.
void my_quicksort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  shuffle(arr, item_size, n);
  
  my_quicksort_recursive(arr, item_size, less, 0, n - 1);
}

// Recursive call to the quicksort algorithm.
void my_quicksort_recursive(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi) {
  if (hi <= lo) {
    return;
  }

  int j = partition(arr, item_size, less, lo, hi);
  
  my_quicksort_recursive(arr, item_size, less, lo, j - 1);  // sort left half
  my_quicksort_recursive(arr, item_size, less, j + 1, hi);  // sort right half
}

// Performs heapsort by first arranging the input array in heap-order, then
// repeatedly exchanging the first (largest) element of the heap with the last
// element, reducing the heap size by 1, and calling "sink" on the new first
// element. This has the effect of progressively placing items in sorted order
// from the end of the array to the front.
void my_heapsort(void *arr, size_t item_size, size_t n, bool (*less)(void *, void *)) {
  // To use basic heap functions, we need the array to start at index 1. This
  // allocates memory and shifts the position of the array by 1 index. This is 
  // not memory efficient. Note that the first element is at position 1, and the
  // last element at position n.
  void *temp_arr = malloc(item_size * (n + 1));
  if (!temp_arr) {
    perror("Failed to malloc");
    exit(EXIT_FAILURE);
  }
  memcpy(temp_arr + item_size, arr, item_size * n);

  // Construct the heap by arranging the array in heap order. Start from the end
  // of the array and iterate toward the beginning, "sinking" each item as we go.
  // We can actually start halfway through the array as the last half have no
  // children and thus do not require the sink operation to be called on them.
  for (int i = n/2; i >= 1; i--) {
    sink(temp_arr, item_size, i, n, less);
  }

  // Sortdown, but repeatedly exchanging the first (largest) element with the 
  // last.
  int last_idx = n;
  while (last_idx > 1) {
    // Swap the first and last elements. The last element is now in place.
    exchange(temp_arr, item_size, 1, last_idx);

    // Decrement the index of the last item.
    last_idx--;

    // Sink the new first element into its heap-ordered position.
    sink(temp_arr, item_size, 1, last_idx, less);
  }

  // Move the sorted array back to the original array and free the temp sorting
  // array.
  memcpy(arr, temp_arr + item_size, item_size * n);
  free(temp_arr);
}

// Sink helper function for heapsort.
// Moves the item at the given idx downward in the binary heap to
// its heap-ordered position, where it is larger than or equal to both its 
// children.
void sink(void *arr, size_t item_size, int idx, size_t n, bool (*less)(void *, void *)) {
  int child_idx1 = idx * 2;
  int child_idx2 = idx * 2 + 1;
  
  while (idx * 2 <= n) {
    // Only one child to worry about
    if (child_idx2 > n) {
      if (!less(arr + child_idx1 * item_size, arr + idx * item_size)) {
        exchange(arr, item_size, idx, child_idx1);
        idx = child_idx1;
      } else {
        break;
      }
    // Need to check against largest child
    } else {
      // child2 is larger
      if (less(arr + child_idx1 * item_size, arr + child_idx2 * item_size)) {
        // Compare against child2
        if (!less(arr + child_idx2 * item_size, arr + idx * item_size)) {
          exchange(arr, item_size, idx, child_idx2);
          idx = child_idx2;
        } else {
          break;
        }
      // child 1 is larger, so compare against it
      } else if (!less(arr + child_idx1 * item_size, arr + idx * item_size)) {
        exchange(arr, item_size, idx, child_idx1);
        idx = child_idx1;
      } else {
        break;
      }
    }
    child_idx1 = idx * 2;
    child_idx2 = idx * 2 + 1;
  }
}

// Partitions and sorts an array using the element at arr[lo] as the partition.
// Returns the index of the final position of the partioning element.
int partition(void *arr, size_t item_size, bool (*less)(void *, void *), int lo, int hi) {
  // Set left-to-right and right-to-left sscan indices. Because the while loops 
  // below use pre-increment and pre-decrement, the actual comparisons will 
  // start at arr[lo + 1] and arr[hi].
  int i = lo;
  int j = hi + 1;

  void *v = arr + lo * item_size;   // partitioning element

  // Scan from left-to-right until we find an entry >= partioning, and scan from
  // right-to-left until we find an entry <= partioning. Swap these entries.
  // If ever our scan indices cross or either one reaches the end, break.
  // As a final step, swap the partioning element with the final index of the
  // right-hand scan.
  while (true) {
    // Increment the left pointer until we reach an element >= the partioning
    // element
    while (less(arr + (++i) * item_size, v)) {
      // Break if we reach the right end
      if (i == hi) {
        break;
      }
    }

    // Decrement the right pointer until we reach an element >= the partioning
    // element
    while (less(v, arr + (--j) * item_size)) {
      // Break if we reach the left end
      if (j == lo) {
        break;
      }
    }
    // Break if left/right indices have crossed
    if (i >= j) {
      break;
    }
    // Swap the out-of-order elements at index i and j
    exchange(arr, item_size, i, j);
  }

  // Swap the partioning element with the element at index j
  exchange(arr, item_size, lo, j);

  return j;
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

// Randomly shuffles an array in place. Note that for small array sizes this is
// not precisely uniform.
void shuffle(void *arr, size_t item_size, size_t n) {
  srand(time(NULL));  // seed PRNG

  void *tmp = malloc(item_size);
  if (!tmp) {
    perror("Failed to malloc");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < n; i++) {
    int r = i + rand() % (n - i);
    memcpy(tmp, arr + i * item_size, item_size);
    memcpy(arr + i * item_size, arr + r * item_size, item_size);
    memcpy(arr + r * item_size, tmp, item_size);
  }

  free(tmp);
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