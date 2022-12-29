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
// Hash Table Implementations:
// -----------------------------------------
// Two variants of hash tables are implemented: separate chaining using
// linked list symbol tables, and linear probing using open addressing.
// 
// The separate chaining hash table consists of an array of linked list symbol 
// tables, where each node includes the key and value. 
// 
// The linear probing approach involves directly allocating an array of keys and
// values, where collisions are handled by iterating past the hash index to the 
// next empty index. A separate array tracks whether each index is empty or not.
//
// The hash function is provided in lib.c, though ideally it should be able to 
// be client-defined for complex objects.
// 
// Trie Implementations:
// -----------------------------------------
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "symbol_table.h"
#include "../stack/stack.h"
#include "../queue/queue.h"

#define INITIAL_HASH_TABLE_CHAINS 4
#define INITIAL_HASH_TABLE_ARRAY  16
#define MAX_HASH_TABLE_CHAIN_LEN  10

// Node in a binary search tree
typedef struct node node_t;

// Node in a sequential search list
typedef struct list_node list_node_t;

// Node in an r-way trie
typedef struct rway_trie_node rway_trie_node_t;

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

struct rway_trie_node {
  void *value;
  rway_trie_node_t **next; // pointers to next nodes
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
  st_t **hash_table;    // pointer to an array of sequential search STs
  int hash_table_chains;

  // For linear-probing hashtable ST
  int hash_table_array_size;  // size of array
  void *hash_table_keys;   // array of keys
  void *hash_table_values; // array of values
  bool *hash_table_used;  // array indicating whether position in HT is empty

  // For R-way trie type STs
  rway_trie_node_t *trie_root;
  int trie_entries;
  queue_t *trie_iter_q;

  // For all hashtable STs
  int hash_table_entries;
  int curr_hash_iter_idx;
  int curr_hash_iter_count;

  // For all STs
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
void resize_hash_chaining(st_t *st, int new_size);
void resize_hash_probing(st_t *st, int new_size);
bool put_hash(st_t *st, void *key, void *value);
bool get_hash(st_t *st, void *key, void *value_found);

// Private functions for tries
rway_trie_node_t *new_node_trie();
rway_trie_node_t *put_trie(st_t *st, rway_trie_node_t *node, char *key, void *value, int d);
bool get_trie(st_t *st, rway_trie_node_t *node, char *key, void *value_found, int d);
void free_trie(rway_trie_node_t *node);
void trie_collect(st_t *st, rway_trie_node_t *node, char *s);

// Creates a symbol table and returns a pointer to it. Pass in the size of the
// key and value types that will be stored in the symbol table, along with a
// function pointer to a function for comparing keys.
st_t *st_init(size_t key_size, size_t value_size, int (*compare)(void *, void *), enum st_type type) {
  st_t *st = malloc(sizeof(st_t));
  if (!st) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // For list-type STs
  st->first = NULL;
  st->list_size = 0;
  st->curr_list_iter = NULL;

  // For tree-type STs
  st->root = NULL;
  st->curr_tree_iter = NULL;
  st->tree_iter_stack = NULL;

  // For separate-chaining hashtable ST
  st->hash_table = NULL;
  st->hash_table_chains = INITIAL_HASH_TABLE_CHAINS;

  // For linear-probing hashtable ST
  st->hash_table_array_size = INITIAL_HASH_TABLE_ARRAY;
  st->hash_table_keys = NULL;
  st->hash_table_values = NULL;
  st->hash_table_used = NULL;

  // For all hash table STs
  st->hash_table_entries = 0;
  st->curr_hash_iter_idx = 0;
  st->curr_hash_iter_count = 0;
  
  // For R-way trie type STs
  st->trie_root = NULL;
  st->trie_entries = 0;
  st->trie_iter_q = NULL;
  
  // For all STs
  st->key_size = key_size;
  st->value_size = value_size;
  st->compare = compare;
  st->type = type;

  // For hash tables, we need to do some initial setup
  switch (st->type) {
    case HASH_TABLE_CHAINING: 
      st->hash_table = malloc(sizeof(st_t *) * st->hash_table_chains);
      if (!st->hash_table) {
        perror("Failed to malloc\n");
        exit(EXIT_FAILURE);
      }

      // Create a linked list sequential search ST for each element in the hash table
      for (int i = 0; i < st->hash_table_chains; i++) {
        st->hash_table[i] = st_init(key_size, value_size, compare, SEQUENTIAL_SEARCH);
      }
      break;
    case HASH_TABLE_PROBING:
      st->hash_table_keys = malloc(st->key_size * st->hash_table_array_size);
      if (!st->hash_table_keys) {
        perror("Failed to malloc\n");
        exit(EXIT_FAILURE);
      }
      st->hash_table_values = malloc(st->value_size * st->hash_table_array_size);
      if (!st->hash_table_values) {
        perror("Failed to malloc\n");
        exit(EXIT_FAILURE);
      }
      st->hash_table_used = calloc(st->hash_table_array_size, sizeof(bool));
      if (!st->hash_table_used) {
          perror("Failed to malloc\n");
        exit(EXIT_FAILURE);
      }
      break;
    default:
      break;
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

// Creates a new separate chaining hash table of the given size, and iterates 
// over the old table to re-insert all entries.
void resize_hash_chaining(st_t *st, int new_size) {
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

// Creates a new linear probing hash table of the given size, and iterates over 
// the old table to re-insert all entries.
void resize_hash_probing(st_t *st, int new_size) {
  // Create an entirely new symbol table as a temporary placeholder
  st_t *tmp_st = st_init(st->key_size, st->value_size, st->compare, st->type);
  
  // Manually resize the arrays to the new size we need
  tmp_st->hash_table_keys = realloc(tmp_st->hash_table_keys, new_size * st->key_size);
  if (!tmp_st->hash_table_keys) {
    perror("Failed to realloc\n");
    exit(EXIT_FAILURE);
  }
  tmp_st->hash_table_values = realloc(tmp_st->hash_table_values, new_size * st->value_size);
  if (!tmp_st->hash_table_values) {
    perror("Failed to realloc\n");
    exit(EXIT_FAILURE);
  }
  tmp_st->hash_table_used = realloc(tmp_st->hash_table_used, new_size * sizeof(bool));
  if (!tmp_st->hash_table_used) {
    perror("Failed to realloc\n");
    exit(EXIT_FAILURE);
  }
  tmp_st->hash_table_array_size = new_size;

  // Iterate over the old table
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

  while (st_iter_has_next(st)) {
    st_iter_next(st, key);
    st_get(st, key, value);
    // Put key/value pairs into the new table
    put_hash(tmp_st, key, value);
  }

  // Copy over the pointers to the keys, values, used arrays, and other elements
  st->hash_table_array_size = tmp_st->hash_table_array_size;
  st->hash_table_entries = tmp_st->hash_table_entries;
  st->hash_table_keys = tmp_st->hash_table_keys;
  st->hash_table_values = tmp_st->hash_table_values;
  st->hash_table_used = tmp_st->hash_table_used;

  // Free the temp symbol table struct
  free(tmp_st);
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

// Returns a new trie node
rway_trie_node_t *new_node_trie() {
  rway_trie_node_t *node = malloc(sizeof(rway_trie_node_t));
  if (!node) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  
  node->value = NULL;
  
  // R-way array of pointers to the next nodes
  node->next = calloc(TRIE_RWAY_RADIX, sizeof(rway_trie_node_t *));
  if (!node->next) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
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
    case TRIE_RWAY:
      st->trie_root = put_trie(st, st->trie_root, *(char **)key, value, 0);
      break;
    case SEQUENTIAL_SEARCH:
      put_list(st, key, value);
      break;
    case HASH_TABLE_CHAINING:
      // Enlargen the table if avg chain length is > MAX_HASH_TABLE_CHAIN_LEN
      if (st->hash_table_entries / st->hash_table_chains > MAX_HASH_TABLE_CHAIN_LEN) {
        resize_hash_chaining(st, 2 * st->hash_table_chains);
      }
      if (put_list(st->hash_table[hash_compute(st, key) % st->hash_table_chains], key, value)) {
        st->hash_table_entries++;
      }
      break;
    case HASH_TABLE_PROBING:
      // Keep load factor between 25% and 50% by doubling size of array whenever 
      // we hit 50% loading.
      if (st->hash_table_entries >= st->hash_table_array_size / 2) {
        resize_hash_probing(st, 2 * st->hash_table_array_size);
      }
      put_hash(st, key, value);
      
      break;
    default:
      printf("Unrecognized type\n");
      return false;
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

// Recursive call to put a string key and value in the subtrie rooted at node.
// Returns the node corresponding to the key. This may be a new (leaf) node if 
// the key did not exist in the tree, or an internal node if the key already 
// existed. In the latter case, the value is updated.
// Takes an index d to determine where in the r-way next array to visit, and
// when to stop (when the end of the string is reached).
rway_trie_node_t *put_trie(st_t *st, rway_trie_node_t *node, char *key, void *value, int d) {
  if (node == NULL) {
    node = new_node_trie();
  }

  if (d == strlen(key)) {
    // We've reached the end of the string so can put our value here
    if (node->value) {
      // Free an existing entry
      free(node->value);
    } else {
      // For a new entry, update our count
      st->trie_entries++;
    }
    node->value = malloc(st->value_size);
    if (!node->value) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
    memcpy(node->value, value, st->value_size);
  } else {
    // We need to continue further down the trie
    node->next[(int)key[d]] = put_trie(st, node->next[(int)key[d]], key, value, d + 1);
  }

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

// Puts a key value pair in a hash table linear probing array. Returns true if
// successful, false otherwise.
bool put_hash(st_t *st, void *key, void *value) {
  // Iterate over hash table indices until we find an empty entry. Note that the
  // resizing above should ensure we find an empty spot eventually.
  int idx;
  for (idx = hash_compute(st, key) % st->hash_table_array_size;
      st->hash_table_used[idx];
      idx = (idx + 1) % st->hash_table_array_size) {
  
    // Check if the key here matches the put key
    void *curr_key = st->hash_table_keys + idx * st->key_size;
    if (st->compare(curr_key, key) == 0) {
      // Found existing key, update value and return
      memcpy(st->hash_table_values + idx * st->value_size, value, st->value_size);
      return true;
    }
  }
      
  // Found an empty index, so copy the key/value and update the empty array
  memcpy(st->hash_table_keys + idx * st->key_size, key, st->key_size);
  memcpy(st->hash_table_values + idx * st->value_size, value, st->value_size);
  st->hash_table_used[idx] = true;
  st->hash_table_entries++;
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
    case TRIE_RWAY:
      return get_trie(st, st->trie_root, *(char **)key, value_found, 0);
    case SEQUENTIAL_SEARCH:
      return get_list(st, key, value_found);
    case HASH_TABLE_CHAINING:
      return get_list(st->hash_table[hash_compute(st, key) % st->hash_table_chains], key, value_found);
    case HASH_TABLE_PROBING:
      return get_hash(st, key, value_found);
    default:
      printf("Unrecognized type\n");
      return false;
  }
}

// Returns true if the key exists in the symbol table, false otherwise.
bool st_contains(st_t *st, void *key) {
  void *tmp = malloc(st->value_size);
  if (!tmp) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }
  return st_get(st, key, tmp);
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

// Recursive call to find a given key in the subtrie rooted at node. If the key
// is found, copies the value into value_found and returns true. Otherwise
// returns false. Takes a d index to identify where in the string we are.
bool get_trie(st_t *st, rway_trie_node_t *node, char *key, void *value_found, int d) {
  if (node == NULL) {
    // Key was not found, so return false
    return false;
  }

  if (d == strlen(key) && node->value) {
    // If we have reached the end of the string key and there is a value at this
    // node, copy it
    memcpy(value_found, node->value, st->value_size);
    return true;  
  } else {
    // Otherwise, continue down the trie
    return get_trie(st, node->next[(int)key[d]], key, value_found, d + 1);
  }
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

// Finds a given key in a linear probing hash table. If the key is found, copies 
// the value into value_found and returns true. Otherwise returns false.
bool get_hash(st_t *st, void *key, void *value_found) {
  if (st_is_empty(st)) {
    return false;
  }

  int idx;

  // Iterate over the hash table entries looking for a matching key
  for (idx = hash_compute(st, key) % st->hash_table_array_size;
      st->hash_table_used[idx];
      idx = (idx + 1) % st->hash_table_entries) {
    // Check the key
    void *curr_key = st->hash_table_keys + idx * st->key_size;
    if (st->compare(curr_key, key) == 0) {
      memcpy(value_found, st->hash_table_values + idx * st->value_size, st->value_size);
      return true;
    }
  }

  // We hit an empty index before finding the key so we return. Note that the
  // hash table resizing in st_put() ensures there will be < 100% loading.
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
    case TRIE_RWAY:
      return st->trie_entries;
    case SEQUENTIAL_SEARCH:
      return st->list_size;
    case HASH_TABLE_CHAINING:
    case HASH_TABLE_PROBING:
      return st->hash_table_entries;
    default:
      printf("Unrecognized type\n");
      return -1;
  }
}

// Recursively frees memory associated with the subtree rooted at node.
void free_tree(node_t *node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free(node->key);
  free(node->value);
  free_tree(node->right);
}

// Recursively frees memory associated with the subtrie rooted at node.
void free_trie(rway_trie_node_t *node) {
  if (node == NULL) {
    return;
  }
  for (int i = 0; i < TRIE_RWAY_RADIX; i++) {
    free_trie(node->next[i]);
  }
  free(node->value);
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
    case TRIE_RWAY:
      free_trie(st->trie_root);
      break;
    case SEQUENTIAL_SEARCH:
      free_list(st->first);
      break;
    case HASH_TABLE_CHAINING:
      for (int i = 0; i < st->hash_table_chains; i++) {
        free_list(st->hash_table[i]->first);
      }
      free(st->hash_table);
      break;
    case HASH_TABLE_PROBING:
      free(st->hash_table_keys);
      free(st->hash_table_values);
      free(st->hash_table_used);
      break;
    default:
      printf("Unrecognized type\n");
      return;
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
    case TRIE_RWAY:
      if (st->trie_iter_q) {
        queue_free(st->trie_iter_q);
      }
      st->trie_iter_q = queue_init(sizeof(char *));
      trie_collect(st, st->trie_root, "");
      queue_iter_init(st->trie_iter_q);
      break;
    case SEQUENTIAL_SEARCH:
      st->curr_list_iter = st->first;
      break;
    case HASH_TABLE_CHAINING:
      st->curr_hash_iter_idx = 0;
      st->curr_hash_iter_count = 0;
      st->curr_list_iter = st->hash_table[st->curr_hash_iter_idx]->first;
      break;
    case HASH_TABLE_PROBING:
      st->curr_hash_iter_idx = 0;
      // Set idx to first non-empty index
      while (st->hash_table_entries > 0 && !st->hash_table_used[st->curr_hash_iter_idx]) {
        st->curr_hash_iter_idx++;
      }
      st->curr_hash_iter_count = 0;
      break;
    default:
      printf("Unrecognized type\n");
      return false;
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
    case TRIE_RWAY:
      return queue_iter_has_next(st->trie_iter_q);
    case SEQUENTIAL_SEARCH:
      return st->curr_list_iter != NULL;
    case HASH_TABLE_CHAINING:
    case HASH_TABLE_PROBING:
      return (st->curr_hash_iter_count < st->hash_table_entries);
    default:
      printf("Unrecognized type\n");
      return false;
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
    case TRIE_RWAY:
      queue_iter_next(st->trie_iter_q, key);
      break;
    case SEQUENTIAL_SEARCH:
      memcpy(key, st->curr_list_iter->key, st->key_size);
      st->curr_list_iter = st->curr_list_iter->next;
      break;
    case HASH_TABLE_CHAINING:
      // We've reached the end of the current list, so move to the next one
      while (!st->curr_list_iter) {
        st->curr_hash_iter_idx++;
        st->curr_list_iter = st->hash_table[st->curr_hash_iter_idx]->first;
      }
      memcpy(key, st->curr_list_iter->key, st->key_size);
      st->curr_list_iter = st->curr_list_iter->next;
      st->curr_hash_iter_count++;
      break;
    case HASH_TABLE_PROBING:
      memcpy(key, st->hash_table_keys + st->curr_hash_iter_idx * st->key_size, st->key_size);
      st->curr_hash_iter_count++;
      st->curr_hash_iter_idx++;

      // Find the next non-empty index
      while (!st->hash_table_used[st->curr_hash_iter_idx]) {
        st->curr_hash_iter_idx = (st->curr_hash_iter_idx + 1) % st->hash_table_array_size;
      }
      break;
    default:
      printf("Unrecognized type\n");
      return;
  }
}

// Trie functions below

// Recursively traverses the subtrie rooted at node and adds characters to the
// string s. When a full string key is found, adds it to the trie_iter_q.
void trie_collect(st_t *st, rway_trie_node_t *node, char *s) {
  if (node == NULL) {
    return;
  }

  // We have a full string key, so add it to the queue
  if (node->value) {
    char *str = strdup(s);
    queue_enqueue(st->trie_iter_q, &str);
  }

  // Recursive call to continue down the trie
  for (int i = 0; i < TRIE_RWAY_RADIX; i++) {
    // Grow s to make room for a new character
    char *s_new = malloc(strlen(s) + 2);
    if (!s_new) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
    strcpy(s_new, s);
    s_new[strlen(s)] = i; // add the new character
    s_new[strlen(s) + 1] = '\0';  // terminate
    trie_collect(st, node->next[i], s_new);
    free(s_new);
  }
}

// Finds the longest key that is a prefix of s and copies a pointer to it to 
// key. The caller is responsible for freeing memory associated with the 
// key. Returns true if successful, false otherwise.
bool st_longest_prefix_of(st_t *st, char *s, char **key) {

  
  return false;
}

// Initializes an iterator for examining all keys having s as a prefix.
bool st_prefix_iter_init(st_t *st, char *s) {
  return false;
}

// Returns true if the iterator has more keys, false otherwise.
bool st_prefix_iter_has_next(st_t *st) {
  return false;
}

// Copies a pointer to the next string key to key. The caller is responsible
// for freeing memory associated with the key. Returns true if successful, false
// otherwise.
bool st_prefix_iter_next(st_t *st, char **key) {
  return false;
}

// Initializes an iterator for examining all keys that match s, where "."
// matches any character.
bool st_match_iter_init(st_t *st, char *s) {
  return false;
}

// Returns true if the iterator has more keys, false otherwise.
bool st_match_iter_has_next(st_t *st) {
  return false;
}

// Copies a pointer to the next string key to key. The caller is responsible
// for freeing memory associated with the key. Returns true if successful, false
// otherwise.
char *st_match_iter_next(st_t *st, char **key) {
  return false;
}