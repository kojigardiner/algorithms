// Implementation for a zipping and unzipping a file.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "../lib/lib.h"
#include "../bit_io/bit_io.h"
#include "zip.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define RLE_BITS 8
#define RLE_MAX_RUN_LEN 255

// Private functions
void compress_rle(const char *in_filename, const char *out_filename);
void expand_rle(const char *in_filename, const char *out_filename);
void compress_huffman(const char *in_filename, const char *out_filename);
void expand_huffman(const char *in_filename, const char *out_filename);
void compress_lzw(const char *in_filename, const char *out_filename);
void expand_lzw(const char *in_filename, const char *out_filename);
double filesize_ratio(const char *in_filename, const char *out_filename);

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

void compress_huffman(const char *in_filename, const char *out_filename) {
}

void expand_huffman(const char *in_filename, const char *out_filename) {
}

void compress_lzw(const char *in_filename, const char *out_filename) {
}

void expand_lzw(const char *in_filename, const char *out_filename) {
}