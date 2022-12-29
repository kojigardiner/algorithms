// Interface for a generic symbol table data type with iteration support.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdbool.h>
#include "../lib/lib.h"

enum st_type {
  SEQUENTIAL_SEARCH,
  BST,
  RED_BLACK_BST,
  HASH_TABLE_CHAINING,
  HASH_TABLE_PROBING,
};

// Generic symbol table data type
typedef struct st st_t;

// Creates a symbol table and returns a pointer to it. Pass in the size of the
// key and value types that will be stored in the symbol table, along with a
// function pointer to a function for comparing keys.
// Pass in an st_type parameter to define the type of symbol table to use.
st_t *st_init(size_t key_size, size_t value_size, int (*compare)(void *, void *), enum st_type type);

// Puts a key value pair in the symbol table. Returns true if successful, false
// otherwise. Note that null keys and values are not allowed.
bool st_put(st_t *st, void *key, void *value);

// Returns the value paired with the given key. Copies the value into the
// memory location provided in value_found. The caller is responsible for
// allocating memory for the returned value. Returns true if the key exists,
// false otherwise.
bool st_get(st_t *st, void *key, void *value_found);

// Returns true if the symbol table is empty, false otherwise.
bool st_is_empty(st_t *st);

// Returns the number of key-value pairs in the symbol table.
unsigned int st_size(st_t *st);

// Frees memory associated with the symbol table.
void st_free(st_t *st);

// Initializes an iterator for examining the keys of the symbol table.
// Returns true if the iterator is successfully created, false otherwise.
bool st_iter_init(st_t *st);

// Returns true if the iterator has more keys, false otherwise.
bool st_iter_has_next(st_t *st);

// Copies the next key in the queue to the memory address of key.
void st_iter_next(st_t *st, void *key);
