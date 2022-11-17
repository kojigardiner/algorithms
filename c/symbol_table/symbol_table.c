// Implementation of a generic symbol table data type with iteration support.
// Uses a binary search tree (BST) to store nodes containing key-value pairs.
// Recursive methods for put() and get() traverse the tree in order to find
// the matching key.
//
// This implementation supports unbalanced BSTs and balanced, red-black BSTs,
// which maintain tree balance by simulating a 2-3 tree where a node may have
// either 2 or 3 children. This greatly improves efficiency in cases where keys
// are inserted in sorted (or near-sorted) order, which would result in an
// extremely deep tree structure if not for balancing.
//
// The iterator makes use of a stack to perform in-order traversal of the tree.
// Nodes that are visited but not returned are stored on the stack, allowing
// nodes to be visited in-order (i.e. giving preference to left, root, then 
// right).
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "symbol_table.h"
#include "../stack/stack.h"

// Node in a binary search tree
typedef struct node node_t;

const bool RED = true;
const bool BLACK = false;

struct node {
  node_t *left;
  node_t *right;
  void *key;
  void *value;
  int n;
  bool color;
};

// Generic symbol table data type
typedef struct st {
  node_t *root;
  size_t key_size;
  size_t value_size;
  int (*compare)(void *, void *);
  node_t *curr_iter;
  my_stack_t *iter_stack;
  enum st_type type;
} st_t;

// Private functions
node_t *put(st_t *st, node_t *node, void *key, void *value);
bool get(st_t *st, node_t *node, void *key, void *value_found);
node_t *new_node(st_t *st, void *key, void *value);
int size(node_t *node);
void free_tree(node_t *node);

// Private functions for re-orienting nodes in a red-black BST
bool is_red(node_t *node);
void flip_colors(node_t *h);
node_t *rotate_left(node_t *h);
node_t *rotate_right(node_t *h);

// Creates a symbol table and returns a pointer to it. Pass in the size of the
// key and value types that will be stored in the symbol table, along with a
// function pointer to a function for comparing keys.
st_t *st_init(size_t key_size, size_t value_size, int (*compare)(void *, void *), enum st_type type) {
  st_t *st = malloc(sizeof(st_t));
  if (!st) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  st->root = NULL;
  st->key_size = key_size;
  st->value_size = value_size;
  st->compare = compare;
  st->curr_iter = NULL;
  st->type = type;

  return st;
}

// Returns a new node with the given key and value.
node_t *new_node(st_t *st, void *key, void *value) {
  // Found a null link in the tree, so create a new node
  node_t *node = malloc(sizeof(node_t));
  if (!node) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  node->key = malloc(st->key_size);
  node->value = malloc(st->value_size);
  if (!node->key || !node->value) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  memcpy(node->key, key, st->key_size);
  memcpy(node->value, value, st->value_size);
  node->left = NULL;
  node->right = NULL;
  node->n = 1;

  if (st->type == RED_BLACK_BST) {
    node->color = RED;
  }

  return node;
}

// Returns true if the parent link to this node is red, false otherwise.
bool is_red(node_t *h) {
  if (!h) {
    return false;
  }
  return (h->color == RED);
}

// Flips the colors for a node whose child links are both red.
void flip_colors(node_t *h) {
  h->left->color = BLACK;
  h->right->color = BLACK;
  h->color = RED;
}

// Rotates a node's red right link to its left side, preserving order.
//    Starting point, with a right-leaning red link:
//          E(h)
//         / \\.
//        D   S(x)
//           / \.
//          R   T
//
//    Ending point, with the resulting left-leaning red link:
//          S(x)
//         //  \.
//        E(h)  T
//       / \.
//      D   R
node_t *rotate_left(node_t *h) {
  node_t *x = h->right;   // the node at the right red link
  
  // Update positions of nodes
  h->right = x->left;
  x->left = h;

  // Update colors of links
  x->color = h->color;
  h->color = RED;

  // Update the node counts
  x->n = h->n;  // since the overall size of the subtree has not changed, we
                // can just update the count directly
  h->n = 1 + size(h->left) + size(h->right);  // the lower subtree has changed,
                                              // so we need to calculate here

  // Return a pointer to the new root
  return x;
}

// Rotates a node's red left link to its right side, preserving order.
//    Starting point, with a left-leaning red link:
//          S(h)
//         //  \.
//        E(x)  T
//       / \.
//      D   R
//
//    Ending point, with the resulting right-leaning red link:
//          E(x)
//         / \\.
//        D   S(h)
//           / \.
//          R   T
node_t *rotate_right(node_t *h) {
  node_t *x = h->left;  // the node at the left red link

  // Update positions of the nodes
  h->left = x->right;
  x->right = h;

  // Update colors of links
  x->color = h->color;
  h->color = RED;

  // Update the node counts
  x->n = h->n;  // since the overall size of the subtree has not changed, we
                // can just update the count directly
  h->n = 1 + size(h->left) + size(h->right);  // the lower subtree has changed,
                                              // so we need to calculate here

  // Return a pointer to the new root
  return x;
}


// Returns the size of the subtree rooted at node.
int size(node_t *node) {
  if (!node) {
    return 0;
  }
  return node->n;
}

// Puts a key value pair in the symbol table. Returns true if successful, false
// otherwise. Note that null keys and values are not allowed.
bool st_put(st_t *st, void *key, void *value) {
  if (!st || !key || !value) {
    return false;
  }
  
  st->root = put(st, st->root, key, value);
  if (st->type == RED_BLACK_BST) {
    st->root->color = BLACK;  // preserve black for the root
  }

  return true;
}

// Recursive call to put a key-value pair in the subtree rooted at node. Returns
// the node corresponding to the key. This may be a new (leaf) node if the key 
// did not exist in the tree, or an internal node if the key already existed. In
// the latter case, the value is updated.
node_t *put(st_t *st, node_t *node, void *key, void *value) {
  if (node == NULL) {
    return new_node(st, key, value);
  }

  // Compare key of current node against the search key. Recursively call put()
  // and update links down the tree if we do not have a match.
  int cmp = st->compare(key, node->key);
  if (cmp < 0) {
    node->left = put(st, node->left, key, value);
  } else if (cmp > 0) {
    node->right = put(st, node->right, key, value);
  } else {
    // If we reached here, we have a key match, so update the value.
    memcpy(node->value, value, st->value_size);
  }

  // Update tree structure to preserve left-leaning red links
  if (st->type == RED_BLACK_BST) {
    // Right-leaning red link
    if (is_red(node->right) && !is_red(node->left)) {
      node = rotate_left(node);
    }
    // Two left-leaning red links in a row
    if (is_red(node->left) && is_red(node->left->left)) {
      node = rotate_right(node);
    }
    // Two red links to children
    if (is_red(node->left) && is_red(node->right)) {
      flip_colors(node);
    }
  }
  
  // Update the size of this node based on the size of its subtrees. Given the
  // recursive nature of put(), this will update the size of each node as we 
  // return "back up" the tree.
  node->n = size(node->left) + size(node->right) + 1;
  return node;  
}

// Returns the value paired with the given key. Copies the value into the
// memory location provided in value_found. The caller is responsible for
// allocating memory for the returned value. Returns true if the key exists,
// false otherwise.
bool st_get(st_t *st, void *key, void *value_found) {
  if (!st || !key) {
    return false;
  }

  return get(st, st->root, key, value_found);
}

// Recursive call to find a given key in the subtree rooted at node. If the key
// is found, copies the value into value_found and returns true. Otherwise
// returns false.
bool get(st_t *st, node_t *node, void *key, void *value_found) {
  if (node == NULL) {
    // Key was not found, so return false
    return false;
  }

  // Compare key of current node to determine if we have a hit or need to 
  // continue traversing the tree.
  int cmp = st->compare(key, node->key);
  if (cmp < 0) {
    return get(st, node->left, key, value_found);
  }
  if (cmp > 0) {
    return get(st, node->right, key, value_found);
  }

  // If we get here, we have a key match, so copy the value at the node.
  memcpy(value_found, node->value, st->value_size);
  return true;
}

// Returns true if the symbol table is empty, false otherwise.
bool st_is_empty(st_t *st) {
  if (!st) {
    return false;
  }
  return (st_size(st) == 0);
}

// Returns the number of key-value pairs in the symbol table.
unsigned int st_size(st_t *st) {
  if (!st) {
    return 0;
  }
  return size(st->root);
}

// Recursively frees memory assoicated with the subtree rooted at node.
void free_tree(node_t *node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free(node->key);
  free(node->value);
  free_tree(node->right);
}

// Frees memory associated with the symbol table.
void st_free(st_t *st) {
  // Iterate and free all nodes as well as the st
  free_tree(st->root);
  free(st);

  return;
}

// Initializes an iterator for examining the keys of the symbol table. For
// in-order traversal, we will start with the left-most node in the tree. 
// Returns true if the iterator is successfully created, false otherwise.
bool st_iter_init(st_t *st) {
  if (st_is_empty(st)) {
    return false;
  }
  st->iter_stack = stack_init(sizeof(node_t *));
  if (!st->iter_stack) {
    return false;
  }

  st->curr_iter = st->root;

  return true;
}

// Returns true if the iterator has more keys, false otherwise.
bool st_iter_has_next(st_t *st) {
  if (st_is_empty(st)) {
    return false;
  }
  return (st->curr_iter != NULL || !stack_is_empty(st->iter_stack));
}

// Copies the next key in the tree to the memory address of key.
void st_iter_next(st_t *st, void *key) {
  // If we have hit a null node AND the stack is empty, we are done traversing
  // the tree.
  while (!stack_is_empty(st->iter_stack) || st->curr_iter != NULL) {
    // Traverse left links until we hit a null node. Push all nodes along the
    // way to the stack.
    if (st->curr_iter != NULL) {
      stack_push(st->iter_stack, &st->curr_iter);
      st->curr_iter = st->curr_iter->left;
    // If we hit a null node, pop the last node and copy its key. Set the
    // iterator node to the right child and return. This prepares the next
    // call to iter_next to start traversing the left-hand links of this new
    // node.
    } else {
      stack_pop(st->iter_stack, &st->curr_iter);
      memcpy(key, st->curr_iter->key, st->key_size);
      st->curr_iter = st->curr_iter->right;
      
      return;
    }
  }
}
