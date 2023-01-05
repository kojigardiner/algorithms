// Interface for using regular expressions to search text.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdbool.h>

// Regex data type
typedef struct regex regex_t;

// Initialize a regex struct with a given expression.
regex_t *regex_init(char *regex);

// Returns true if the given text is recognized by the regex, otherwise returns
// false.
bool regex_recognizes(regex_t *re, char *text);

// Frees memory associated with the regex struct.
void regex_free(regex_t *re);