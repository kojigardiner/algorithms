// Interface for a generic stack data type with iteration support.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stddef.h>
#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Generic stack data type.
typedef struct stack my_stack_t;

// Creates an empty stack that holds elements of a given item_size. Returns a
// pointer to the stack. Must call stack_free() when finished using the stack.
my_stack_t *stack_init(size_t item_size);

// Pushes an item onto the stack.
void stack_push(my_stack_t *s, void *item);

// Pops an item from the stack and copies it to the memory address of item.
// Returns true if successful, false otherwise.
bool stack_pop(my_stack_t *s, void *item);

// Returns true if the stack is empty, false otherwise.
bool stack_is_empty(my_stack_t *s);

// Returns the number of items on the stack.
unsigned int stack_size(my_stack_t *s);

// Frees memory associated with stack.
void stack_free(my_stack_t *s);

// Initializes an iterator for examining contents of the stack.
void stack_iter_init(my_stack_t *s);

// Returns true if the iterator has more items, false otherwise.
bool stack_iter_has_next(my_stack_t *s);

// Copies the next item in the stack to the memory address of item.
void stack_iter_next(my_stack_t *s, void *item);
