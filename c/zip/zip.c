// Implementation for a zipping and unzipping a file. Includes Run Length
// Encoding, Huffman compression, and LZW compression.
// 
// Run Length Encoding
// -----------------------------
// Takes advantage of the fact that certain types of files have long sequences 
// of consecutive 0s and 1s, and represents those sequences with a single value
// representing their count. Works best on binary bitmaps (e.g. 0 = black, 1 =
// white) but is not as efficient for text files, which often lack runs of
// consecutive bits.
//
// Compression:
// Writes the count of runs of bits that have the same value. By default the
// count is written out as an 8-bit value, and can therefore encode runs with
// max length of 255. By convention, the first value written is the number of
// continuous zeros, followed by the number of continuous ones and so on. In
// cases where a run exceeds length of 255, 255 is written, then zero, then
// the remaining run count.
//
// Expansion:
// Reads run counts from the compressed file, and writes the appropriate 
// sequence of 0s and 1s.
//
// Huffman Compression
// -----------------------------
// Uses a per-file variable-length encoding to represent frequently seen bit
// patterns with fewer bits. In this implementation, the file is examined in
// 8-bit sequences, and a set of prefix-free codes is assigned to each sequence
// based on its frequency, using a binary trie. The trie is written as a header
// in the compressed file, which makes huffman compression less valuable for
// small filesizes, where the trie itself could increase the filesize.
//
// Compression:
// Requires two passes through the input file: first to compute frequencies
// of each 8-bit sequence and build the encoding trie, and second, to read the 
// input and convert each 8-bit sequence it its new encoding. 
// 
// The trie is built by using a min priority queue initialized with a node for 
// every 8-bit sequence and its frequency. The two nodes with smallest frequency 
// are removed and a new node added that has the two as children and a frequency 
// that is the sum of its children. The process repeats until there is one node 
// left, the root. This trie is then written to the file, followed by the total
// number of 8-bit sequences in the original file.
//
// The resulting trie is then traversed to build an encoding table, where moving 
// to a left child in the trie represents adding a '0' to the code, and moving
// right represents adding a '1' to the code, until a leaf node is reached.
// 
// Expansion:
// Reads the trie and total byte count from the compressed file, then reads
// successive bits from the file, following the trie from root to leaf node
// based on the bit value ('0' -> go left, '1' -> go right). The 8-bit sequence
// corresponding to the leaf node is written to the output file.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "../lib/lib.h"
#include "../bit_io/bit_io.h"
#include "../priority_queue/priority_queue.h"
#include "../symbol_table/symbol_table.h"
#include "zip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define RLE_BITS 8  // number of bits used to encode run length
#define RLE_MAX_RUN_LEN ((1 << RLE_BITS) - 1) // max run length

#define HUFFMAN_MAX_CODE_LEN 256  // max code length (255 + null-terminator)
#define HUFFMAN_RADIX 256         // extended ASCII alphabet

#define LZW_CODE_WIDTH 12         // bits for codes
#define LZW_MAX_CODES ((1 << LZW_CODE_WIDTH) - 1)     // max codes (4095)
#define LZW_EOF_CODE TRIE_RWAY_RADIX   // EOF code

// Private functions
double filesize_ratio(const char *in_filename, const char *out_filename);

// RLE helper functions
void compress_rle(const char *in_filename, const char *out_filename);
void expand_rle(const char *in_filename, const char *out_filename);

// Huffman helper functions
void compress_huffman(const char *in_filename, const char *out_filename);
void expand_huffman(const char *in_filename, const char *out_filename);

typedef struct node_huffman node_huffman_t;

struct node_huffman {
  int c;      // character code at the node, '\0' if none
  int freq;   // frequency of character at node OR sum of freqs below the node
  node_huffman_t *left;   // node associated with current code + '0', NULL if leaf
  node_huffman_t *right;  // node associated with current code + '1', NULL if leaf
};

node_huffman_t *make_node_huffman(int c, int freq, node_huffman_t *left, node_huffman_t *right);
bool is_leaf_huffman(node_huffman_t *node);

char **build_code_table_huffman(node_huffman_t *node);
void build_code_table_huffman_recursive(node_huffman_t *node, char *s, char **table);
void free_code_table_huffman(char **table);

node_huffman_t *build_trie_huffman(int *freqs);
void free_trie_huffman(node_huffman_t *node);
void write_trie_huffman(node_huffman_t *node, bit_io_t *b);
node_huffman_t *read_trie_huffman(bit_io_t *b);

bool less_node_huffman(void *a, void *b);

// LZW helper functions
void compress_lzw(const char *in_filename, const char *out_filename);
void expand_lzw(const char *in_filename, const char *out_filename);

void write_code_lzw(bit_io_t *b, int code);
int read_code_lzw(bit_io_t *b);

st_t *reset_compress_table_lzw(st_t *st);
char **reset_expand_table_lzw(char **table);

// Compresses a file with the given compression method. Returns the compression
// ratio achieved.
double compress(const char *in_filename, const char *out_filename, enum zip_type type) {
  switch (type) {
    case RLE:
      compress_rle(in_filename, out_filename);
      break; 
    case HUFFMAN:
      compress_huffman(in_filename, out_filename);
      break;
    case LZW:
      compress_lzw(in_filename, out_filename);
      break;
    default:
      return 0;
  }

  return filesize_ratio(in_filename, out_filename);
}

// Expands a file with the given decompression method.
void expand(const char *in_filename, const char *out_filename, enum zip_type type) {
  switch (type) {
    case RLE:
      expand_rle(in_filename, out_filename);
      break;
    case HUFFMAN:
      expand_huffman(in_filename, out_filename);
      break;
    case LZW:
      expand_lzw(in_filename, out_filename);
      break;
    default:
      return;
  }
}

// Verifies that two files have the same contents
bool verify(const char *filename1, const char *filename2) {
  FILE *fp1 = fopen(filename1, "r");
  if (!fp1) {
    printf("Failed to open %s\n", filename1);
    return false;
  }
  FILE *fp2 = fopen(filename2, "r");
  if (!fp2) {
    printf("Failed to open %s\n", filename2);
    return false;
  }
  
  // Check filesizes
  struct stat stat1, stat2;
  stat(filename1, &stat1);
  stat(filename2, &stat2);
  if (stat1.st_size != stat2.st_size) {
    printf("Filesizes are not the same\n");
    return false;
  }

  // Read and compare every byte
  uint8_t data1, data2;
  for (int i = 0; i < stat1.st_size; i++) {
    fread(&data1, 1, 1, fp1);
    fread(&data2, 1, 1, fp2);
    if (data1 != data2) {
      fclose(fp1);
      fclose(fp2);
      printf("Verify failed on %d of %lld\n", i, stat1.st_size);
      return false;
    }
  }
  return true;
}

// Returns the ratio between the input and output filesizes
double filesize_ratio(const char *in_filename, const char *out_filename) {
  struct stat s1, s2;
  
  if (stat(in_filename, &s1) != 0) {
    return 0;
  }
  if (stat(out_filename, &s2) != 0) {
    return 0;
  }

  return (double)s2.st_size / (double)s1.st_size;
}

/* Run Length Encoding */

// Compresses the given input file using RLE and writes it to the given output 
// file.
void compress_rle(const char *in_filename, const char *out_filename) {
  // Create binary reader and writer
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");

  // Start with run of 0s
  bool last_bit = false;
  bool curr_bit;
  uint8_t count = 0;
  
  while (!bit_io_eof(bin)) {
    curr_bit = bit_io_read_bit(bin);

    // If bit pattern has changed, write the current count and reset the count
    if (curr_bit != last_bit) {
      bit_io_write_bits(bout, count, RLE_BITS);
      count = 0;
    // If the bit pattern is the same but we hit the max count, write the count,
    // write a 0 count, and reset the counter
    } else if (count == RLE_MAX_RUN_LEN) {
      bit_io_write_bits(bout, count, RLE_BITS);
      count = 0;
      bit_io_write_bits(bout, count, RLE_BITS);
    }

    last_bit = curr_bit;

    count++;
  }

  // Write the last count
  bit_io_write_bits(bout, count, RLE_BITS);

  // Close the files
  bit_io_close(bin);
  bit_io_close(bout);
}

// Expands the given input file using RLE and writes it to the given output 
// file.
void expand_rle(const char *in_filename, const char *out_filename) {
  // Create binary reader and writer
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");

  // Start with run of 0s
  bool curr_bit = false;
  uint8_t count;

  while (!bit_io_eof(bin)) {
    // Read the current count
    count = bit_io_read_bits(bin, RLE_BITS);

    // Write out count bits
    for (int i = 0; i < count; i++) {
      bit_io_write_bit(bout, curr_bit);
    }

    // Flip bits
    curr_bit = !curr_bit;
  }

  // Close files
  bit_io_close(bin);
  bit_io_close(bout);
}

/* Huffman Compression */

// Compresses the input file using huffman encoding and writes it to the output
// filename.
void compress_huffman(const char *in_filename, const char *out_filename) {
  // Create reader/writer bit_io
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");

  int *freqs = calloc(HUFFMAN_RADIX, sizeof(int));
  if (!freqs) {
    perror("Failed to calloc\n");
    exit(EXIT_FAILURE);
  }

  int total = 0;
  // Read input (pass 1) to build frequencies table
  while (!bit_io_eof(bin)) {
    uint8_t c = bit_io_read_byte(bin);
    freqs[c]++;
    total++;
  }

  // Build trie
  node_huffman_t *root = build_trie_huffman(freqs);

  // Build code table
  char **table = build_code_table_huffman(root);

  // Write trie
  write_trie_huffman(root, bout);

  // Write total number of characters
  bit_io_write_int(bout, total);

  // Close and reopen file
  bit_io_close(bin);
  bin = bit_io_open(in_filename, "r");

  // Read input (pass 2) and write code for each character.
  while (!bit_io_eof(bin)) {
    // Get the character
    uint8_t c = bit_io_read_byte(bin);

    // Get the bitstring code for the character
    char *s = table[c];
    if (!s) {
      printf("Bitstring for %d not in table!\n", c);
    }

    // Write each bit of the code
    for (int i = 0; i < strlen(s); i++) {
      if (s[i] == '1') {
        bit_io_write_bit(bout, true);
      } else {
        bit_io_write_bit(bout, false);
      }
    }
  }

  // Clean up
  free(freqs);
  free_code_table_huffman(table);
  free_trie_huffman(root);

  bit_io_close(bin);
  bit_io_close(bout);
}

// Expands the input file that has been encoded using huffman compression, and
// writes the output to out_filename.
void expand_huffman(const char *in_filename, const char *out_filename) {
  // Create reader/writer bit_io
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");
  
  // Read trie
  node_huffman_t *root = read_trie_huffman(bin);

  // Read count of characters
  int total = bit_io_read_int(bin);

  // For each character we expect in the file, decode bits with trie
  node_huffman_t *node = root;
  for (int i = 0; i < total; i++) {
    // Read bits until we hit a leaf node
    while (!is_leaf_huffman(node)) {
      if (bit_io_read_bit(bin)) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    // Write the character at the leaf node and reset the node to root
    bit_io_write_byte(bout, (uint8_t)node->c);
    node = root;
  }

  // Clean up
  free_trie_huffman(root);
  bit_io_close(bin);
  bit_io_close(bout);
}

// Makes a huffman node with the given parameters and returns it. The caller
// is responsible for freeing the node.
node_huffman_t *make_node_huffman(int c, int freq, node_huffman_t *left, node_huffman_t *right) {
  node_huffman_t *node = malloc(sizeof(node_huffman_t));
  if (!node) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  node->c = c;
  node->freq = freq;
  node->left = left;
  node->right = right;

  return node;
}

// Returns true if the given node is a leaf node (no children), false otherwise.
bool is_leaf_huffman(node_huffman_t *node) {
  return (node->left == NULL && node->right == NULL);
}

// Returns a table indexed by character value, containing bitstrings 
// representing each character. Traverses the trie rooted at node to build the
// table. The caller is responsible for freeing the memory associated with the 
// returned table.
char **build_code_table_huffman(node_huffman_t *node) {
  // Create a table of strings, with number of rows defined by the alphabet
  // radix.
  char **table = calloc(HUFFMAN_RADIX, sizeof(char *));
  if (!table) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Will hold the initial empty string
  char *s = strdup("");
  if (!s) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  // Recursive call to build the table
  build_code_table_huffman_recursive(node, s, table);
  free(s);

  return table;
}

// Recursively traverses the trie and adds huffman codes to the table. Takes
// a pointer to the current node in the trie, a string of the code up to that
// node, and a pointer to the table containing completed codes.
void build_code_table_huffman_recursive(node_huffman_t *node, char *s, char **table) {
  // When we hit a leaf node, we can add the code to the table.
  if (is_leaf_huffman(node)) {
    // Dupe the string as the original will be freed by previous recursive calls
    char *store_s = strdup(s);
    if (!store_s) {
      perror("Failed to malloc\n");
      exit(EXIT_FAILURE);
    }
    table[(int)node->c] = store_s;
    return;
  }

  // Recur down left and right child links. A new string is allocated for each,
  // as they will end up becoming separate codes.
  int curr_len = strlen(s);
  
  // +1 for new bit, +1 for null-terminator
  char *left_s = calloc(curr_len + 2, sizeof(char));
  strcpy(left_s, s);
  left_s[curr_len] = '0';
  build_code_table_huffman_recursive(node->left, left_s, table);
  free(left_s);

  // +1 for new bit, +1 for null-terminator
  char *right_s = calloc(curr_len + 2, sizeof(char));  
  strcpy(right_s, s);
  right_s[curr_len] = '1';
  build_code_table_huffman_recursive(node->right, right_s, table);
  free(right_s);
}

// Frees memory associated with the huffman code table.
void free_code_table_huffman(char **table) {
  for (int i = 0; i < HUFFMAN_RADIX; i++) {
    if (table[i]) {
      free(table[i]); // free the bitstring
    }
  }
  free(table);
}

// Returns the root node of a huffman trie corresponding to the given character
// frequency array.
node_huffman_t *build_trie_huffman(int *freqs) {
  // Create a priority queue and insert nodes into it.
  // The number of nodes for a huffman trie with N characters is 2 * N - 1.
  pq_t *pq = pq_init(sizeof(node_huffman_t *), 2 * HUFFMAN_RADIX, MIN, less_node_huffman);
  for (int c = 0; c < HUFFMAN_RADIX; c++) {
    if (freqs[c] > 0) {
      node_huffman_t *node = make_node_huffman(c, freqs[c], NULL, NULL);
      pq_insert(pq, &node);
    }
  }

  // Iterate over the priority queue, removing the two nodes with the smallest
  // frequencies and creating a parent node that points to them and has as its
  // frequency the sum of its children's. Continue until there is only one node 
  // remaining, the root.
  while (pq_size(pq) > 1) {
    node_huffman_t *a;
    node_huffman_t *b;

    pq_del(pq, &a);
    pq_del(pq, &b);

    node_huffman_t *parent = make_node_huffman('\0', a->freq + b->freq, a, b);
    pq_insert(pq, &parent);
  }

  node_huffman_t *root;
  pq_del(pq, &root);

  return root;
}

// Recursively frees memory associated with the huffman trie rooted a node.
void free_trie_huffman(node_huffman_t *node) {
  if (is_leaf_huffman(node)) {
    free(node);
    return;
  }

  free_trie_huffman(node->left);
  free_trie_huffman(node->right);
  free(node);

  return;
}

// Writes the huffman trie rooted at node to the given bit_io output stream.
// Writes a zero bit for internal trie nodes, and a one bit followed by the
// 8-bit ASCII code for a character for a leaf node. Does a recursive preorder 
// traversal of the trie, by writing the current node values before visiting
// the children.
void write_trie_huffman(node_huffman_t *node, bit_io_t *b) {
  // Leaf node
  if (is_leaf_huffman(node)) {
    bit_io_write_bit(b, true);
    bit_io_write_byte(b, (uint8_t)node->c);
    return;
  } 
  
  // Internal node
  bit_io_write_bit(b, false);
  write_trie_huffman(node->left, b);
  write_trie_huffman(node->right, b);

  return;
}

// Reads a huffman trie from the given bit_io output stream and recursively
// constructs the trie, returning the root node to the initial caller. A zero
// bit indicates an internal trie node and a one bit indicates a leaf node, with
// the following byte representing that node's character. Note that the
// frequencies have no meaning in nodes built from a file-read trie, so they
// are set arbitrarily to 0.
node_huffman_t *read_trie_huffman(bit_io_t *b) { 
  // Leaf node
  if (bit_io_read_bit(b)) {
    uint8_t c = bit_io_read_byte(b);
    node_huffman_t *node = make_node_huffman(c, 0, NULL, NULL);

    return node;
  }
  
  // Internal node
  node_huffman_t *node = make_node_huffman('\0', 0, NULL, NULL);
  node->left = read_trie_huffman(b);
  node->right = read_trie_huffman(b);

  return node;
}

// Returns true if the frequency of the huffman node a is less than that of b,
// false otherwise.
bool less_node_huffman(void *a, void *b) {
  node_huffman_t *n1 = *(node_huffman_t **)a;
  node_huffman_t *n2 = *(node_huffman_t **)b;
  return (n1->freq < n2->freq);
}

/* LZW compression */

void compress_lzw(const char *in_filename, const char *out_filename) {
  // Create reader/writer
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");

  // Setup a symbol table with compression codes
  st_t *st = reset_compress_table_lzw(NULL);

  // Keep track of used codes. EOF is TRIE_RWAY_RADIX, so one beyond that is 
  // where the rest of our codes start.
  int code_counter = LZW_EOF_CODE + 1;

  // Read the entire file into memory as a string
  struct stat stat1;
  stat(in_filename, &stat1);
  char *full_file = calloc(stat1.st_size + 1, sizeof(char)); // add 1 for final null-terminator
  if (!full_file) {
    perror("Failed to calloc\n");
    exit(EXIT_FAILURE);
  }
  int count = 0;
  
  while (!bit_io_eof(bin)) {
    uint8_t byte = bit_io_read_byte(bin);
    full_file[count] = byte;
    count++;
  }
  
  // Iterate over file contents while there are still unscanned bytes
  int keylen = 0;
  for (int position = 0; position < stat1.st_size; position += keylen) {
    if (code_counter == LZW_MAX_CODES) {
      // Reset symbol table and counter
      st = reset_compress_table_lzw(st);
      code_counter = LZW_EOF_CODE + 1;
    }

    // Find the longest prefix of the unscanned file that exists in the symbol
    // table. At the very least, we should find one character.
    // NOTE: because binary files may have arbitrarily placed null-bytes, this
    // method will only find prefixes up-to such null-bytes, due to C-string
    // processing.
    char *key;
    int code;

    // Special case if current position is a null byte
    if (full_file[position] == '\0') {
      write_code_lzw(bout, 0);
      keylen = 1;
      continue;
    }

    // Special case if there is only one char remaining, skip the prefix check
    if (full_file[position + 1] == '\0') {
      key = strdup(&full_file[position]);
      if (!key) {
        perror("Failed to strdup\n");
        exit(EXIT_FAILURE);
      }
    } else if (!st_longest_prefix_of(st, &full_file[position], &key)) {
      printf("Failed to find any prefix in symbol table!\n");
      exit(EXIT_FAILURE);
    }

    // Get the code for that prefix key
    if (!st_get(st, &key, &code)) {
      printf("Failed to find %s key in symbol table!\n", key);
    }

    // Write the code
    write_code_lzw(bout, code);

    // Move the position pointer
    keylen = strlen(key);

    // Add a new code to the symbol table if we have more codes available and
    // the lookahead character is in range and the lookahead isn't a null byte
    int lookahead = position + keylen;
    if (code_counter < LZW_MAX_CODES && lookahead < stat1.st_size && full_file[lookahead] != '\0') {
      // Create a new key and add the next character to it
      char *new_key = calloc(keylen + 1, sizeof(char));
      if (!new_key) {
        perror("Failed to calloc\n");
        exit(EXIT_FAILURE);
      }
      strcpy(new_key, key);
      new_key[keylen] = full_file[lookahead];

      // Add the new key and increment the code count
      st_put(st, &new_key, &code_counter);
      code_counter++;

      free(new_key);
    }

    free(key);
  }

  write_code_lzw(bout, LZW_EOF_CODE);

  // Clean up
  st_free(st);
  free(full_file);
  bit_io_close(bin);
  bit_io_close(bout);
}

void expand_lzw(const char *in_filename, const char *out_filename) {
  // Create reader/write
  bit_io_t *bin = bit_io_open(in_filename, "r");
  bit_io_t *bout = bit_io_open(out_filename, "w");

  // Set up a table indexed by LZW code that provides the associated character
  // byte string.
  char **table = reset_expand_table_lzw(NULL);

  // Keep track of used codes. EOF is TRIE_RWAY_RADIX, so one beyond that is 
  // where the rest of our codes start.
  int code_counter = LZW_EOF_CODE + 1;

  // Allocate space for two max code strings
  char *curr = calloc(LZW_MAX_CODES + 1, sizeof(unsigned char));
  char *next = calloc(LZW_MAX_CODES + 1, sizeof(unsigned char));

  // Process the first character
  int code = read_code_lzw(bin);
  strcpy(curr, table[code]);
  
  // Process the rest of the file
  while (true) {
    if (code_counter == LZW_MAX_CODES) {
      // printf("Expanded max number of codes!\n");
      table = reset_expand_table_lzw(table);
      code_counter = LZW_EOF_CODE + 1;
    }

    // Write all bytes of the current string
    
    // Special case where we write a null byte
    if (strlen(curr) == 0) {
      bit_io_write_byte(bout, 0);
    } else {
      for (int i = 0; i < strlen(curr); i++) {
        bit_io_write_byte(bout, curr[i]);
      }
    }

    // Read the next code
    code = read_code_lzw(bin);
    if (code == LZW_EOF_CODE) {
      break;
    }

    // Special case where code is 0, set string to empty and continue
    if (code == 0) {
      strcpy(curr, "");
      continue;
    }
    
    // Special case where we don't yet have the current code in our table, so
    // we have to create it
    if (!table[code]) {
      char *new_codestring = calloc(strlen(curr) + 2, sizeof(char));
      if (!new_codestring) {
        perror("Failed to calloc\n");
        exit(EXIT_FAILURE);
      }
      strcpy(new_codestring, curr);

      // Add the first letter of the current string to the new codestring
      new_codestring[strlen(curr)] = curr[0];
      table[code_counter++] = new_codestring;
      strcpy(next, table[code]);

    // If we do have the code in our table, update the next string with it
    } else {
      strcpy(next, table[code]);

      // Lookahead in next, and add a new code to our table if we have space
      // in the table and the current string isn't empty.
      if (code_counter < LZW_MAX_CODES && strlen(curr) > 0) {
        char *new_codestring = calloc(strlen(curr) + 2, sizeof(char));
        if (!new_codestring) {
          perror("Failed to calloc\n");
          exit(EXIT_FAILURE);
        }
        strcpy(new_codestring, curr);

        // Add the first letter of the next string to the new codestring
        new_codestring[strlen(curr)] = next[0];
        table[code_counter++] = new_codestring;
      }
    }
  
    // Replace contents of curr with next so it gets written out in the next
    // iteration.
    strcpy(curr, next);
  }

  // Cleanup
  free(next);
  free(curr);

  for (int i = 0; i < LZW_MAX_CODES; i++) {
    if (table[i]) {
      free(table[i]);
    }
  }
  free(table);

  bit_io_close(bin);
  bit_io_close(bout);
}

// Writes the bits of an LZW code to the file associated with b.
void write_code_lzw(bit_io_t *b, int code) {
  // Write MSB first
  for (int i = LZW_CODE_WIDTH - 1; i >= 0; i--) {
    bit_io_write_bit(b, (code & (1 << i)) > 0);
  }
}

// Reads the bits of an LZW code to the file associated with b.
int read_code_lzw(bit_io_t *b) {
  int code = 0;
  // Read MSB first
  for (int i = LZW_CODE_WIDTH - 1; i >= 0; i--) {
    code <<= 1;
    code |= bit_io_read_bit(b);
  }
  return code;
}

// Resets and returns a pointer to a symbol table for LZW compression. Pass
// in a pointer to a previous symbol table to be replaced, or NULL if this is
// the first call.
st_t *reset_compress_table_lzw(st_t *old_st) {
  if (old_st) {
    st_free(old_st);
  }

  // Create symbol table (trie) for mapping strings (byte sequences) to codes
  st_t *st = st_init(sizeof(char *), sizeof(int), compare_str, TRIE_RWAY);

  // Initialize symbol table with single byte strings = ASCII value code
  for (int i = 0; i < TRIE_RWAY_RADIX; i++) {
    char *s = calloc(2, sizeof(char));
    s[0] = (unsigned char)i;
    st_put(st, &s, &i);
    free(s);
  }

  return st;
}

// Resets and returns a pointer to a table for LZW expansion. Pass
// in a pointer to a previous table to be replaced, or NULL if this is
// the first call.
char **reset_expand_table_lzw(char **old_table) {
  // Free the entries > TRIE_RWAY_RADIX for an old table
  if (old_table) {
    for (int i = TRIE_RWAY_RADIX; i < LZW_MAX_CODES; i++) {
      if (old_table[i]) {
        free(old_table[i]);
        old_table[i] = NULL;
      }
    }
    return old_table;
  }

  // Create table of codewords
  char **table = calloc(LZW_MAX_CODES, sizeof(char *));
  
  // Allocate single character strings for each codeword
  for (int i = 0; i < TRIE_RWAY_RADIX; i++) {
    char *s = calloc(2, sizeof(char));
    s[0] = (unsigned char)i;
    table[i] = s;
  }

  return table;
}