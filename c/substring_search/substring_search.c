// Implementation of four algorithms for finding the first occurrence of a
// substring in a given string of text.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "substring_search.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const long long PRIME = 257492101824743;

// Private functions
int substring_search_brute(char *pattern, char *text);
int substring_search_kmp(char *pattern, char *text);
int substring_search_boyer_moore(char *pattern, char *text);
int substring_search_rabin_karp(char *pattern, char *text);

void compute_dfa(char *pattern, int **dfa);
int **calloc_2d_array(int rows, int cols);
void free_2d_array(int **arr, int rows, int cols);
void print_dfa(char *pattern, int **dfa);
long long str_hash(char *str, int m);

// Search for the given pattern in the given text. Returns the starting position
// of the pattern if found, or -1 otherwise.
int substring_search(char *pattern, char *text, enum search_type type) {
  switch (type) {
    case BRUTE:
      return substring_search_brute(pattern, text);
    case KMP:
      return substring_search_kmp(pattern, text);
    case BOYER_MOORE:
      return substring_search_boyer_moore(pattern, text);
    case RABIN_KARP:
      return substring_search_rabin_karp(pattern, text);
    default:
      return -1;
  }
}

// Uses brute-force method to use each character in the text as a starting 
// position to compare against the pattern.
int substring_search_brute(char *pattern, char *text) {
  int n = strlen(text);
  int m = strlen(pattern);

  int i, j;
  for (i = 0; i <= n - m; i++) {
    for (j = 0; j < m; j++) {
      // Compare "ahead" by starting at current ith text position and comparing
      // pattern characters until we find a mismatch
      if (pattern[j] != text[i + j]) {
        break;
      }
    }
    // If we made it all the way to the end of the pattern, return the position
    // in the text where we started to match
    if (j == m) {
      return i;
    }
  }

  // No match found, so return -1
  return -1;
}

// Allocates and clears a 2D array with the given number of rows and columns.
// The resulting array can be accessed with double brackets (e.g. arr[][]).
// Caller is responsible for freeing the array with free_2d_array().
int **calloc_2d_array(int rows, int cols) {
  int **arr = calloc(rows, sizeof(int *));
  if (!arr) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < rows; i++) {
    arr[i] = calloc(cols, sizeof(int));
    if (!arr[i]) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
  }

  return arr;
}

// Frees memory associated with a 2D array created with calloc_2d_array().
void free_2d_array(int **arr, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    free(arr[i]);
  }
  free(arr);
}

// Prints the contents of a DFA formed from a given pattern.
void print_dfa(char *pattern, int **dfa) {
  int m = strlen(pattern);

  printf("%4c", ' ');
  for (int j = 0; j < m; j++) {
    printf("%4c", pattern[j]);
  }
  printf("\n");

  // Print only the alphabet characters
  for (int c = 'A'; c <= 'Z'; c++) {
    printf("%4c", c);
    for (int j = 0; j < m; j++) {
      printf("%4d", dfa[c][j]);
    }
    printf("\n");
  }
}

// Computes the KMP DFA state machine for the given pattern and stores it in
// the 2D dfa array passed by the caller.
//
// The 2D array is defined such that each row represents a potential character
// seen in the input, and each column represents the corresponding match state
// for the pattern. 
// 
// In the simplest case of all matches, each cell of dfa[char][pattern_idx] will
// simply contain the subsequent index of the pattern. When there is a mismatch,
// we can copy over the contents of a previous column of the dfa, corresponding
// to a previous state. In the worst case, we revert back to the starting state;
// however, in some cases we may be able to return to an intermediate state.
//
// This "backup" in the dfa is accomplished by keeping track of a dfa index
// pointer "x" that lags the pattern by one cycle, and thus tracks the position
// we would have been in if the pattern matching started one character later.
// This allows us to easily extract the dfa state corresponding to a mismatched
// character.
void compute_dfa(char *pattern, int **dfa) {
  int m = strlen(pattern);
  int x = 0;  // pointer to "lag" state of dfa

  // Iterate over the pattern
  for (int j = 0; j < m; j++) {
    // Fill in next state for all characters
    for (int c = 0; c < RADIX; c++) {
      if (c == pattern[j]) {
        dfa[c][j] = j + 1;      // match: move to the next state
      } else {
        dfa[c][j] = dfa[c][x];  // mismatch: copy next state values from state x
      }
    }
    // update x once we are past the first character of the pattern
    if (j > 0) {
      x = dfa[(int)pattern[j]][x];
    }
  }

  // print_dfa(pattern, dfa);
}

// Knuth-Morris-Pratt algorithm for substring search using a DFA state machine.
// See compute_dfa() for details on the DFA construction. Once constructed,
// we can simply iterate over the input text and update the DFA state until
// either the final matched "halt" state or the end of the text is reached.
//
// Returns the index of the first matched character of the pattern, or -1 if
// there is no match.
int substring_search_kmp(char *pattern, char *text) {
  int n = strlen(text);
  int m = strlen(pattern);

  int **dfa = calloc_2d_array(RADIX, m + 1); // + 1 for halt state
  compute_dfa(pattern, dfa);

  int i;
  int j = 0;

  for (i = 0; i < n; i++) {
    j = dfa[(int)text[i]][j];
    if (j == m) { // halt state
      break;
    }
  }

  free_2d_array(dfa, RADIX, m);

  if (i == n) {       // reached the end of the text
    return -1;
  } else {
    return i - m + 1; // first matched character
  }
}

// Boyer-Moore algorithm for substring search using the mismatched character
// heuristic. Instead of matching text to pattern from left-to-right as in the
// brute force method, we move through the text left-to-right but look for
// matches in the pattern moving right-to-left. This allows us to skip ahead as 
// soon as a mismatch is found.
//
// The distance to skip ahead is aided by the right[] array, which stores
// the rightmost index of each character in the pattern. This allows us to 
// breakdown the mismatched character heuristic as follows:
//    1) If the mismatched character is not in the pattern, simply shift our
//       text pointer past the mismatched character.
//    2) If the mismatched character is in the pattern and its rightmost
//       occurrence is to the left of the current pattern character, shift
//       the text such that the characters align.
//    3) If the mismatched character is in the pattern and its rightmost
//       occurrence is to the right of the current pattern character, we just
//       move the text by one position to the let the text "catch up".
//
// Returns the index of the first matched character of the pattern, or -1 if
// there is no match.
int substring_search_boyer_moore(char *pattern, char *text) {
  int n = strlen(text);
  int m = strlen(pattern);

  // Compute skip array, which is the index of the rightmost occurrence of a
  // character in the pattern.
  int *right = malloc(RADIX * sizeof(int));
  // Default value is -1
  for (int c = 0; c < RADIX; c++) {
    right[c] = -1;
  }
  // Update characters in the pattern
  for (int j = 0; j < m; j++) {
    right[(int)pattern[j]] = j;
  }

  // Iterate over the text from l to r, and the pattern from r to l
  int i, j;
  for (i = 0; i <= n - m; i++) {
    for (j = m - 1; j >= 0; j--) {
      // printf("%d %d\n", i, i+j);

      char c = text[i + j];
      // Matched!
      if (c == pattern[j] && j == 0) {
        free(right);
        return i;
      }
      // Mismatched
      if (c != pattern[j]) {
        int mismatch_idx = right[(int)c];
        // Mismatched character is not in the pattern, so shift the text past
        // the pattern and break
        if (mismatch_idx == -1) {
          i += j;
          break;
        }
        // Mismatched character is to the left of the current pattern character,
        // so shift the text and break
        if (mismatch_idx < j) {
          i += j - mismatch_idx - 1;
          break;
        }
        // Mismatched character is to the right of the current pattern character,
        // so simply break and allow the text to shift by one character
        if (mismatch_idx > j) {
          break;
        }
      }
    }
  }

  return -1;
}

// Computes the hash of a given string, using the first m characters and 
// Horner's method to treat a string as a multi-digit number with radix RADIX.
long long str_hash(char *str, int m) {
  long long h = 0;
  for (int i = 0; i < m; i++) {
    h = (h * RADIX + (long long)str[i]) % PRIME;
  }
  
  return h;
}

// Rabin-Karp algorithm for substring search, which computes a hash
// "fingerprint" for a pattern and uses a variation of Horner's method to 
// efficiently calculate successive hashes of characters in the text, searching
// for a match.
// 
// Returns the index of the first matched character of the pattern, or -1 if
// there is no match.
int substring_search_rabin_karp(char *pattern, char *text) {
  int m = strlen(pattern);
  int n = strlen(text);

  // Compute radix multiplier used to subtract the leading "digit"
  // Equivalent to RADIX ^ (m - 1)
  long long rm = 1;
  for (int i = 0; i < m - 1; i++) {
    rm  = (rm * RADIX) % PRIME;
  }

  // Compute the pattern hash and the hash of the first m characters of text
  long long pat_hash = str_hash(pattern, m);
  long long h = str_hash(text, m);

  // Immediate match, return
  if (h == pat_hash) {
    return 0;
  }
  
  // Check successive characters in the text by removing the leading character
  // and adding the next character. Take care when performing modulus operations
  // mid-calculation!
  for (int i = 1; i <= n - m; i++) {
    h = (h + PRIME - (text[i - 1] * rm) % PRIME) % PRIME;   // add PRIME before calc to prevent negative result
    h = ((h * RADIX) % PRIME + text[i + m - 1]) % PRIME;

    if (h == pat_hash) {
      return i;
    }
  }

  return -1;
}