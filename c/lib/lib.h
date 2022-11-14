// Library of shared functions for algorithm implementations.

#pragma once

#include <stdbool.h>

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Comparison functions for commmon types. Comparison functions for more
// complex types must be provided by the client.
bool less_int(void *v, void *w);
bool less_uint(void *v, void *w);
bool less_float(void *v, void *w);
bool less_double(void *v, void *w);
bool less_char(void *v, void *w);
bool less_str(void *v, void *w);