// Interface for reading and writing binary data to a file with single bit
// granularity. Assumes a little-endian system.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#pragma once

#include <stdlib.h>
#include <stdbool.h>

// Bit IO data type
typedef struct bit_io bit_io_t;

// Initializes a bit_io struct with the given filename and returns a pointer to
// it.
bit_io_t *bit_io_open(const char *filename, const char *mode);

// Writes the specified data type to the file stream associated with the
// bit_io_t struct. Note that data to-be-written is buffered and may not be 
// written out until bit_io_close is explicitly called.

// Writes one bit.
void bit_io_write_bit(bit_io_t *b,  bool data);

// Writes the least significant r bits of data.
void bit_io_write_bits(bit_io_t *b, uint8_t data, size_t r);

// Writes the full byte of data.
void bit_io_write_byte(bit_io_t *b, uint8_t data);

// Writes a 4-byte int in little-endian order.
void bit_io_write_int(bit_io_t *b, int data);

// Reads the specified data type from the file stream associated with the
// bit_io_t struct.

// Reads and returns one bit.
bool bit_io_read_bit(bit_io_t *b);

// Reads and returns the least significant r bits.
uint8_t bit_io_read_bits(bit_io_t *b, size_t r);

// Reads and returns a full byte.
uint8_t bit_io_read_byte(bit_io_t *b);

// Reads and returns a 4-byte int assuming little-endian order.
int bit_io_read_int(bit_io_t *b);

// Returns true if the file stream associated with the bit_io_t struct has
// reached the end.
bool bit_io_eof(bit_io_t *b);

// Flushes any buffers to disk and closes the file associated with the given
// bit_io struct. Any incomplete bytes are MSB zero-padded before writing. 
// Also frees any memory associated with the bit_io struct.
void bit_io_close(bit_io_t *b);