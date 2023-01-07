// Implemenation for reading and writing binary data to a file with single bit
// granularity. Assumes a little-endian system.
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include "bit_io.h"
#include "../lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Private functions
static void bit_io_flush(bit_io_t *b);
static void bit_io_fill(bit_io_t *b);
// static uint8_t reverse_byte(uint8_t byte);

// Bit IO data type
typedef struct bit_io {
  bool is_read_mode;      // was file opened in read mode?
  FILE *fp;
  uint8_t buffer;         // buffer for byte to be read/written
  size_t bits_in_buffer;  // number of valid bits in the buffer
} bit_io_t;

// Initializes a bit_io struct with the given filename and returns a pointer to
// it.
bit_io_t *bit_io_open(const char *filename, const char *mode) {
  bit_io_t *b = malloc(sizeof(bit_io_t));
  if (!b) {
    perror("Failed to malloc\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(mode, "r") == 0) {
    b->is_read_mode = true;
  } else if (strcmp(mode, "w") == 0) {
    b->is_read_mode = false;
  } else {
    printf("Mode '%s' not supported\n", mode);
    free(b);
    return NULL;
  }

  b->fp = fopen(filename, mode);
  if (!b->fp) {
    perror("Failed to open file");
    return NULL;
  }

  b->buffer = 0;
  b->bits_in_buffer = 0;

  return b;
}

// Writes the specified data type to the file stream associated with the
// bit_io_t struct. Note that data to-be-written is buffered and may not be 
// written out until bit_io_close is explicitly called.

// Writes one bit.
void bit_io_write_bit(bit_io_t *b,  bool data) {
  if (b->bits_in_buffer == BITS_PER_BYTE) {
    bit_io_flush(b);
  }
  if (data) {
    b->buffer |= (1 << (BITS_PER_BYTE - b->bits_in_buffer - 1));
  }
  b->bits_in_buffer++;
  if (b->bits_in_buffer == BITS_PER_BYTE) {
    bit_io_flush(b);
  }
}

// Writes the least significant r bits of data.
void bit_io_write_bits(bit_io_t *b, uint8_t data, size_t r) {
  if (r == 0 || r > BITS_PER_BYTE) {
    printf("Warning: Data not written. Number of bits must be >0 and <= 8\n");
    return;
  }
  for (int i = 0; i < r; i++) {
    bit_io_write_bit(b, ((data & (1 << i)) >> i));
  }
}

// Writes the full byte of data.
void bit_io_write_byte(bit_io_t *b, uint8_t data) {
  bit_io_write_bits(b, data, BITS_PER_BYTE);
}

// Writes a 4-byte int.
void bit_io_write_int(bit_io_t *b, int data) {
  uint8_t curr_byte;
  for (int i = 0; i < BYTES_PER_INT; i++) {
    curr_byte = ((data & (0xFF << (i * BITS_PER_BYTE))) >> (i * BITS_PER_BYTE)) & 0xFF;
    bit_io_write_byte(b, curr_byte);
  }
}

// Reads the specified data type from the file stream associated with the
// bit_io_t struct.

// Reads and returns one bit.
bool bit_io_read_bit(bit_io_t *b) {
  if (b->bits_in_buffer == 0) {
    bit_io_fill(b);
  }

  bool data = (b->buffer & (1 << (b->bits_in_buffer - 1))) > 0;
  b->bits_in_buffer--;

  return data;
}

// Reads and returns the least significant r bits.
uint8_t bit_io_read_bits(bit_io_t *b, size_t r) {
  if (r == 0 || r > BITS_PER_BYTE) {
    printf("Warning: Data not read. Number of bits must be >0 and <= 8\n");
    return 0;
  }

  uint8_t data = 0;
  for (int i = 0; i < r; i++) {
    if (bit_io_read_bit(b)) {
      data |= (1 << i);
    }
  }

  return data;
}

// Reads and returns a full byte.
uint8_t bit_io_read_byte(bit_io_t *b) {
  return bit_io_read_bits(b, BITS_PER_BYTE);
}

// Reads and returns a 4-byte int.
int bit_io_read_int(bit_io_t *b) {
  int data = 0;

  uint8_t curr_byte;
  for (int i = 0; i < BYTES_PER_INT; i++) {
    curr_byte = bit_io_read_byte(b);
    data |= (curr_byte << i * BITS_PER_BYTE);
  }

  return data;
}

// Returns true if the file stream associated with the bit_io_t struct has
// reached its end.
bool bit_io_eof(bit_io_t *b) {
  if (b->is_read_mode && b->bits_in_buffer == 0) {
    // Check that EOF has been reached AND all bits have been read from the
    // buffer.
    int c = fgetc(b->fp);
    if (c == EOF) {
      return true;
    } else {
      ungetc(c, b->fp);
      return false;
    }
  }
  return false;
}

// Flushes any buffers to disk and closes the file associated with the given
// bit_io struct. Also frees any memory associated with the bit_io struct.
void bit_io_close(bit_io_t *b) {
  bit_io_flush(b);
  fclose(b->fp);
  free(b);
}

// Flushes the current contents of the buffer by calling fwrite. Resets the
// buffer content and pointer.
static void bit_io_flush(bit_io_t *b) {
  if (!b->is_read_mode && b->bits_in_buffer > 0) {
    // b->buffer = reverse_byte(b->buffer);
    if (fwrite(&b->buffer, 1, 1, b->fp) != 1) {
      printf("Failed to write!\n");
      exit(EXIT_FAILURE);
    }
    b->bits_in_buffer = 0;
    b->buffer = 0;
  }
}

// // Reverses the bits in a byte
// static uint8_t reverse_byte(uint8_t byte) {
//   uint8_t reversed = 0;
    
//   reversed |= ((byte & (1 << 0)) << 7);
//   reversed |= ((byte & (1 << 1)) << 5);
//   reversed |= ((byte & (1 << 2)) << 3);
//   reversed |= ((byte & (1 << 3)) << 1);
//   reversed |= ((byte & (1 << 4)) >> 1);
//   reversed |= ((byte & (1 << 5)) >> 3);
//   reversed |= ((byte & (1 << 6)) >> 5);
//   reversed |= ((byte & (1 << 7)) >> 7);

//   return reversed;
// }

// Fills the buffer with new data by calling fread. Resets the buffer pointer.
static void bit_io_fill(bit_io_t *b) {
  if (b->is_read_mode) {
    if (b->bits_in_buffer > 0) {
      printf("Warning! Filling buffer with unread contents\n");
    }
    if (fread(&b->buffer, 1, 1, b->fp) != 1) {
      printf("Failed to read!\n");
      exit(EXIT_FAILURE);
    }
    // b->buffer = reverse_byte(b->buffer);
    b->bits_in_buffer = BITS_PER_BYTE;
  }
}