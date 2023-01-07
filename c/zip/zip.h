// Interface for a zipping and unzipping a file.
// 
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include "../lib/lib.h"

enum zip_type {
  RLE,      // Run-length encoding
  HUFFMAN,  // Huffman compression
  LZW,      // Liv-Zempel compression
};

// Compresses a file with the given compression method. Returns the compression
// ratio achieved.
double compress(const char *in_filename, const char *out_filename, enum zip_type type);

// Expands a file with the given decompression method.
void expand(const char *in_filename, const char *out_filename, enum zip_type type);

// Verifies that two files have the same contents
bool verify(const char *filename1, const char *filename2);