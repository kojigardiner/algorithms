// Implementation for using regular expressions to search text. Uses a NFA (non-
// deterministic finite automaton) to map transitions between states, where
// "epsilon" transitions are stored in a directed graph, and represent state
// transitions that do not consume any input characters. "Match" transitions
// consume an input character and simply move the NFA to the next state.
//
// DFS is used to find all states reachable in the graph from a given state, and
// a bag is used to store the collection of reachable states.
//
// This simple regex does not support square brackets '[]', braces '{}', '+', or
// '?' characters, and requires 
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "regex.h"
#include "../graph/graph.h"
#include "../paths/paths.h"
#include "../stack/stack.h"
#include "../bag/bag.h"

// Regex data type
typedef struct regex {
  graph_t *g;   // graph of epsilon transitions
  char *expr;   // the regex
} regex_t;

// Initialize a regex struct with a given expression.
regex_t *regex_init(char *regex) {
  if (regex == NULL || strlen(regex) == 0) {
    printf("Expression cannot be NULL or the empty string\n");
    return NULL;
  }

  regex_t *re = malloc(sizeof(regex_t));
  if (!re) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // If client did not include outer parentheses, add them
  if (regex[0] != '(') {
    re->expr = calloc(sizeof(char), strlen(regex) + 3); // 2 parens + \0
    if (!re->expr) {
      perror("Failed to calloc\n");
      exit(EXIT_FAILURE);
    }
    re->expr[0] = '(';
    strcpy(&re->expr[1], regex);
    re->expr[strlen(regex) + 1] = ')';
  } else {
    re->expr = strdup(regex);
    if (!re->expr) {
      perror("Failed to calloc\n");
      exit(EXIT_FAILURE);
    }
  }

  // Compile the NFA
  // printf("Compiling: %s\n", re->expr);
  
  int m = strlen(re->expr);
  re->g = graph_init(m + 1, DIRECTED);
  my_stack_t *s = stack_init(sizeof(int));
  int pop;  // index popped from stack
  int lp;
  int or;
  char next;

  for (int i = 0; i < m; i++) {
    next = re->expr[i+1];  // lookahead
    switch (re->expr[i]) {
      case '(':
        graph_add_edge(re->g, i, i + 1);  // move one state
        stack_push(s, &i);                // push to stack
        break;
      case ')':
        graph_add_edge(re->g, i, i + 1);  // move one state
        
        if (!stack_pop(s, &pop)) {
          printf("Stack is empty! Double check the regular expression for mistmatched parentheses\n");
          stack_free(s);
          regex_free(re);
          return NULL;
        }
        if (re->expr[pop] == '|') {
          or = pop;
          graph_add_edge(re->g, or, i);       // from or to the right paren
          
          // next char should be the left paren
          if (!stack_pop(s, &pop)) {
            printf("Stack is empty! Double check the regular expression for mistmatched parentheses\n");
            stack_free(s);
            regex_free(re);
            return NULL;
          }
          lp = pop;
          if (re->expr[lp] != '(') {
            printf("Expected ( but got %c. Cascaded | are not allowed! Consider rewriting the regular expression with explicit () around each two-way |\n", re->expr[lp]);
            stack_free(s);
            regex_free(re);
            return NULL;
          }
          graph_add_edge(re->g, lp, or + 1);  // from left paren to just past the or
        } else {
          lp = pop;
          if (re->expr[lp] != '(') {
            printf("Expected ( but got %c. Double check the regular expression for mistmatched parentheses\n", re->expr[lp]);
            stack_free(s);
            regex_free(re);
            return NULL;
          }
        }

        // closure expression follows the paren, so add edges from left paren
        if (next == '*') {
          graph_add_edge(re->g, lp, i + 1);
          graph_add_edge(re->g, i + 1, lp);
        }
        break;
      case '|':
        stack_push(s, &i);  // push the or
        break;
      case '*':
        graph_add_edge(re->g, i, i + 1);  // move one state
        break;
      case '[':
      case ']':
      case '{':
      case '}':
      case '-':
      case '+':
      case '?':
      case '^':
        printf("%c metacharacter is not supported! Consider rewriting the regular expression using the supported metacharacters: . * | ( )\n", re->expr[i]);
        stack_free(s);
        regex_free(re);
        return NULL;
      default:  // normal alphabet character
        // single-character closure
        if (next == '*') {
          graph_add_edge(re->g, i, i + 1);
          graph_add_edge(re->g, i + 1, i);
        }
        break;
    }
  }

  if (!stack_is_empty(s)) {
    printf("Stack is not empty! Double check the regular expression for mistmatched parentheses\n");
    stack_free(s);
    regex_free(re);
    return NULL;
  }
  // graph_print(re->g);

  return re;
}

// Returns true if the given text is recognized by the regex, otherwise returns
// false.
//
// First finds the set of states reachable from state 0 via epsilon transitions.
//
// Then finds the set of states reachable from the epsilon set via match 
// transitions for the first character in the text. Then finds the set of
// states reachable via epsilon transitions from the match set. If any of this 
// new epsilon set contains the end state, return true. Otherwise, repeat the 
// above with the next character in the text.
bool regex_recognizes(regex_t *re, char *text) {
  int n = strlen(text);
  int m = strlen(re->expr);

  // States reachable via epsilon transitions
  bag_t *eps_states = bag_init(sizeof(int));

  // Examine all states reachable from 0 via epsilon transitions
  paths_t *p = paths_init(re->g, 0, DFS);
  int match;
  int v;
  for (v = 0; v < graph_V(re->g); v++) {
    if (has_path_to(p, v)) {
      // printf("Eps from %d -> %d\n", 0, v);
      bag_add(eps_states, &v);
    }
  }

  for (int i = 0; i < n; i++) {
    // Match transition states
    bag_t *match_states = bag_init(sizeof(int));

    // Iterate over states reachable ia epsilon transitions
    bag_iter_init(eps_states);
    while (bag_iter_has_next(eps_states)) {
      bag_iter_next(eps_states, &v);

      // If the state is not the end state
      if (v <= m) {
        // If the character matches (or wildcard)
        if (re->expr[v] == text[i] || re->expr[v] == '.') {
          // Add a match state transition
          match = v + 1;
          // printf("Match '%c' from %d -> %d\n", text[i], v, match);
          bag_add(match_states, &match);
        }
      }
    }

    // Reset the epsilon states
    bag_free(eps_states);
    eps_states = bag_init(sizeof(int));

    // Recalc reachable epsilon states for each of the match states  
    bag_iter_init(match_states);
    while (bag_iter_has_next(match_states)) {
      paths_free(p);
      bag_iter_next(match_states, &match);
      p = paths_init(re->g, match, DFS);
      for (v = 0; v < graph_V(re->g); v++) {
        if (has_path_to(p, v)) {
          // printf("Eps add from %d -> %d\n", match, v);
          bag_add(eps_states, &v);
        }
      }
    }
    bag_free(match_states);

    // Check the reachable states to see if the end state is within it
    bag_iter_init(eps_states);
    while (bag_iter_has_next(eps_states)) {
      bag_iter_next(eps_states, &v);
      if (v == m) {
        bag_free(eps_states);
        paths_free(p);
        return true;
      }
    }
  }

  bag_free(eps_states);
  paths_free(p);
  return false;
}

// Frees memory associated with the regex struct.
void regex_free(regex_t *re) {
  graph_free(re->g);
  free(re->expr);
  free(re);
}