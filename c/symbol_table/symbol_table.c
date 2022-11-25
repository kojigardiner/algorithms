// Implementation of a generic symbol table data type with iteration support.
// Four implementations are provided, two based on binary search trees, one 
// based on a unordered singly linked list, and one based on a hash table with
// separate chaining.
// 
// Binary Search Tree (BST) Implementations:
// -----------------------------------------
// Stores nodes containing key-value pairs in a tree.
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
// Hash Table and Sequential Search:
// -----------------------------------------
// The hash table consists of an array of linked list symbol tables, where each
// node includes the key and value. The hash function is provided in lib.c,
// though ideally it should be able to be client-defined for complex objects.
//
// An alternative to the separate chaining approach implemented 
// here is the linear probing approach, which involves directly allocating an
// array of keys and values, where collisions are handled by iterating past the
// hash index to the next empty index.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "symbol_table.h"
#include "../stack/stack.h"

#define INITIAL_HASH_TABLE_CHAINS 4
#define MAX_HASH_TABLE_CHAIN_LEN  10

// Node in a binary search tree
typedef struct node node_t;

// Node in a sequential search list
typedef struct list_node list_node_t;

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

struct list_node {
  void *key;
  void *value;
  list_node_t *next;
};

// Generic symbol table data type
typedef struct st {
  // For list-type STs
  list_node_t *first;
  int list_size;
  list_node_t *curr_list_iter;

  // For tree-type STs
  node_t *root;
  node_t *curr_tree_iter;
  my_stack_t *tree_iter_stack;

  // For separate-chaining hashtable ST
  st_t **hash_table;
  int hash_table_size;
  int hash_table_chains;
  int curr_hash_iter_idx;
  int curr_hash_iter_count;

  size_t key_size;
  size_t value_size;
  int (*compare)(void *, void *);
  enum st_type type;
} st_t;

// Private functions for trees
node_t *put_tree(st_t *st, node_t *node, void *key, void *value);
bool get_tree(st_t *st, node_t *node, void *key, void *value_found);
node_t *new_node_tree(st_t *st, void *key, void *value);
int size_tree(node_t *node);
void free_tree(node_t *node);

// Private functions for re-orienting nodes in a red-black BST
bool is_red(node_t *node);
void flip_colors(node_t *h);
node_t *rotate_left(node_t *h);
node_t *rotate_right(node_t *h);

// Private functions for sequential search lists
list_node_t *new_node_list(st_t *st, void *key, void *value);
bool put_list(st_t *st, void *key, void *value);
bool get_list(st_t *st, void *key, void *value_found);
void free_list(list_node_t *node);

// Private functions for hash tables
uint32_t hash_compute(st_t *st, void *key);
void resize_hash(st_t *st, int new_size);

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
  st->first = NULL;
  st->hash_table = NULL;
  st->list_size = 0;
  st->hash_table_size = 0;
  st->hash_table_chains = INITIAL_HASH_TABLE_CHAINS;
  st->key_size = key_size;
  st->value_size = value_size;
  st->compare = compare;
  st->curr_tree_iter = NULL;
  st->curr_list_iter = NULL;
  st->curr_hash_iter_idx = 0;
  st->curr_hash_iter_count = 0;
  st->tree_iter_stack = NULL;
  st->type = type;

  if (st->type == HASH_TABLE) {
    st->hash_table = malloc(sizeof(st_t *) * st->hash_table_chains);
    if (!st->hash_table) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }

    // Create a linked list sequential search ST for each element in the hash table
    for (int i = 0; i < st->hash_table_chains; i++) {
      st->hash_table[i] = st_init(key_size, value_size, compare, SEQUENTIAL_SEARCH);
    }
  }

  return st;
}

// Compute a hash value for the given key. This implementation will work for
// primitives and strings, but not for pointers to objects. A better method
// could be to allow the client to provide their own hash function for complex
// types, similar to the way the "compare" function is user-provided.
uint32_t hash_compute(st_t *st, void *key) {
  if (st->compare == compare_str) {
    char *str = *(char **)key;
    size_t len = strlen(str);
    return fnv_hash_32(str, len);  
  }
  return fnv_hash_32(key, st->key_size);
}

// Creates a new hash table of the given size, and iterates over the old table
// to re-insert all entries.
void resize_hash(st_t *st, int new_size) {
  // Allocate the new table
  st_t **new_hash_table = malloc(sizeof(st_t *) * new_size);
  if (!new_hash_table) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < new_size; i++) {
    new_hash_table[i] = st_init(st->key_size, st->value_size, st->compare, SEQUENTIAL_SEARCH);
  }

  if (!st_iter_init(st)) {
    printf("Failed to create iterator\n");
    exit(EXIT_FAILURE);
  }

  void *key = malloc(st->key_size);
  void *value = malloc(st->value_size);
  if (!key || !value) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Iterate over the old table
  while (st_iter_has_next(st)) {
    st_iter_next(st, key);
    st_get(st, key, value);
    // Put key/value pairs into the new table
    st_put(new_hash_table[hash_compute(st, key) % new_size], key, value);
  }
  
  // Free the old table
  for (int i = 0; i < st->hash_table_chains; i++) {
    st_free(st->hash_table[i]);
  }
  free(st->hash_table);

  // Update size
  st->hash_table = new_hash_table;
  st->hash_table_chains = new_size;
}

// Returns a new tree node with the given key and value.
node_t *new_node_tree(st_t *st, void *key, void *value) {
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

// Returns a new linked list node with the given key and value.
list_node_t *new_node_list(st_t *st, void *key, void *value) {
  list_node_t *node = malloc(sizeof(list_node_t));
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
  node->next = NULL;

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
  h->n = 1 + size_tree(h->left) + size_tree(h->right);  // the lower subtree has changed,
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
  h->n = 1 + size_tree(h->left) + size_tree(h->right);  // the lower subtree has changed,
                                              // so we need to calculate here

  // Return a pointer to the new root
  return x;
}


// Returns the size of the subtree rooted at node.
int size_tree(node_t *node) {
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
  
  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      st->root = put_tree(st, st->root, key, value);
      if (st->type == RED_BLACK_BST) {
        st->root->color = BLACK;  // preserve black for the root
      }
      break;
    case SEQUENTIAL_SEARCH:
      put_list(st, key, value);
      break;
    case HASH_TABLE:
      // Enlargen the table if avg chain length is > MAX_HASH_TABLE_CHAIN_LEN
      if (st->hash_table_size / st->hash_table_chains > MAX_HASH_TABLE_CHAIN_LEN) {
        resize_hash(st, 2 * st->hash_table_chains);
      }
      if (put_list(st->hash_table[hash_compute(st, key) % st->hash_table_chains], key, value)) {
        st->hash_table_size++;
      }
      break;
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }

  return true;
}

// Recursive call to put a key-value pair in the subtree rooted at node. Returns
// the node corresponding to the key. This may be a new (leaf) node if the key 
// did not exist in the tree, or an internal node if the key already existed. In
// the latter case, the value is updated.
node_t *put_tree(st_t *st, node_t *node, void *key, void *value) {
  if (node == NULL) {
    return new_node_tree(st, key, value);
  }

  // Compare key of current node against the search key. Recursively call put()
  // and update links down the tree if we do not have a match.
  int cmp = st->compare(key, node->key);
  if (cmp < 0) {
    node->left = put_tree(st, node->left, key, value);
  } else if (cmp > 0) {
    node->right = put_tree(st, node->right, key, value);
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
  node->n = size_tree(node->left) + size_tree(node->right) + 1;
  return node;  
}

// Puts a key value pair on the linked list. Returns true if a new node is 
// added, false otherwise.
bool put_list(st_t *st, void *key, void *value) {
  for (list_node_t *node = st->first; node != NULL; node = node->next) {
    // Search hit, replace value
    if (st->compare(key, node->key) == 0) {
      memcpy(node->value, value, st->value_size);
      return false;
    }
  }

  // Search miss, add new node to front of list
  list_node_t *new_node = new_node_list(st, key, value);
  new_node->next = st->first;
  st->first = new_node;
  st->list_size++;

  return true;
}

// Returns the value paired with the given key. Copies the value into the
// memory location provided in value_found. The caller is responsible for
// allocating memory for the returned value. Returns true if the key exists,
// false otherwise.
bool st_get(st_t *st, void *key, void *value_found) {
  if (!st || !key) {
    return false;
  }

  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      return get_tree(st, st->root, key, value_found);
    case SEQUENTIAL_SEARCH:
      return get_list(st, key, value_found);
    case HASH_TABLE:
      return get_list(st->hash_table[hash_compute(st, key) % st->hash_table_chains], key, value_found);
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }
}

// Recursive call to find a given key in the subtree rooted at node. If the key
// is found, copies the value into value_found and returns true. Otherwise
// returns false.
bool get_tree(st_t *st, node_t *node, void *key, void *value_found) {
  if (node == NULL) {
    // Key was not found, so return false
    return false;
  }

  // Compare key of current node to determine if we have a hit or need to 
  // continue traversing the tree.
  int cmp = st->compare(key, node->key);
  if (cmp < 0) {
    return get_tree(st, node->left, key, value_found);
  }
  if (cmp > 0) {
    return get_tree(st, node->right, key, value_found);
  }

  // If we get here, we have a key match, so copy the value at the node.
  memcpy(value_found, node->value, st->value_size);
  return true;
}

// Finds a given key in the linked list. If the key is found, copies the value 
// into value_found and returns true. Otherwise returns false.
bool get_list(st_t *st, void *key, void *value_found) {
  for (list_node_t *node = st->first; node != NULL; node = node->next) {
    // Search hit, replace value
    if (st->compare(key, node->key) == 0) {
      memcpy(value_found, node->value, st->value_size);
      return true;
    }
  }
  return false;
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

  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      return size_tree(st->root);
    case SEQUENTIAL_SEARCH:
      return st->list_size;
    case HASH_TABLE:
      return st->hash_table_size;
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }
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

// Frees nodes of a linked list.
void free_list(list_node_t *node) {
  while (node != NULL) {
    free(node->key);
    free(node->value);
    list_node_t *last_node;
    last_node = node;
    node = node->next;
    free(last_node);
  }
}

// Frees memory associated with the symbol table.
void st_free(st_t *st) {
  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      // Iterate and free all nodes
      free_tree(st->root);
      break;
    case SEQUENTIAL_SEARCH:
      free_list(st->first);
      break;
    case HASH_TABLE:
      for (int i = 0; i < st->hash_table_chains; i++) {
        free_list(st->hash_table[i]->first);
      }
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }
  
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

  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      st->tree_iter_stack = stack_init(sizeof(node_t *));
      if (!st->tree_iter_stack) {
        return false;
      }

      st->curr_tree_iter = st->root;
      break;
    case SEQUENTIAL_SEARCH:
      st->curr_list_iter = st->first;
      break;
    case HASH_TABLE:
      st->curr_hash_iter_idx = 0;
      st->curr_hash_iter_count = 0;
      st->curr_list_iter = st->hash_table[st->curr_hash_iter_idx]->first;
      break;
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }

  return true;
}

// Returns true if the iterator has more keys, false otherwise.
bool st_iter_has_next(st_t *st) {
  if (st_is_empty(st)) {
    return false;
  }

  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      return (st->curr_tree_iter != NULL || !stack_is_empty(st->tree_iter_stack));
    case SEQUENTIAL_SEARCH:
      return st->curr_list_iter != NULL;
    case HASH_TABLE:
      return (st->curr_hash_iter_count < st->hash_table_size);
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }
}

// Copies the next key in the tree to the memory address of key.
void st_iter_next(st_t *st, void *key) {
  switch (st->type) {
    case BST:
    case RED_BLACK_BST:
      // If we have hit a null node AND the stack is empty, we are done traversing
      // the tree.
      while (!stack_is_empty(st->tree_iter_stack) || st->curr_tree_iter != NULL) {
        // Traverse left links until we hit a null node. Push all nodes along the
        // way to the stack.
        if (st->curr_tree_iter != NULL) {
          stack_push(st->tree_iter_stack, &st->curr_tree_iter);
          st->curr_tree_iter = st->curr_tree_iter->left;
        // If we hit a null node, pop the last node and copy its key. Set the
        // iterator node to the right child and return. This prepares the next
        // call to iter_next to start traversing the left-hand links of this new
        // node.
        } else {
          stack_pop(st->tree_iter_stack, &st->curr_tree_iter);
          memcpy(key, st->curr_tree_iter->key, st->key_size);
          st->curr_tree_iter = st->curr_tree_iter->right;
          
          return;
        }
      }
      break;
    case SEQUENTIAL_SEARCH:
      memcpy(key, st->curr_list_iter->key, st->key_size);
      st->curr_list_iter = st->curr_list_iter->next;
      break;
    case HASH_TABLE:
      // We've reached the end of the current list, so move to the next one
      while (!st->curr_list_iter) {
        st->curr_hash_iter_idx++;
        st->curr_list_iter = st->hash_table[st->curr_hash_iter_idx]->first;
      }
      memcpy(key, st->curr_list_iter->key, st->key_size);
      st->curr_list_iter = st->curr_list_iter->next;
      st->curr_hash_iter_count++;
      break;
    default:
      printf("Unrecognized type\n");
      exit(EXIT_FAILURE);
  }
}
