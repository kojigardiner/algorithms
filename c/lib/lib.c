// Library of shared functions for algorithm implementations.

#include "lib.h"
#include <stdbool.h>
#include <string.h>

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
