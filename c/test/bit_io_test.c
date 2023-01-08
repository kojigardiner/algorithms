
#include "../../../Unity/src/unity.h"
#include "../bit_io/bit_io.h"
#include <string.h>
#include <stdlib.h>

static char *filename = "bit_io_test_file";
static const uint8_t min_byte = 0;
static const uint8_t max_byte = 255;
static const uint8_t byte_step = 1;
static const int min_int = -2147483648;
static const int max_int = 2147483391;
static const int int_step = 256*256*256 - 1;

void setUp() {
  // Clear the file before each test
  remove(filename);
}

void tearDown() {
}

// Helper functions

// Creates an empty file
void create_file() {
  FILE *fp = fopen(filename, "w");
  fclose(fp);
}

// Creates a file and writes byte values 0 through 255 to it.
void write_file_byte() {
  FILE *fp = fopen(filename, "w");
  for (uint8_t i = min_byte; i < max_byte; i += byte_step) {
    fwrite(&i, sizeof(uint8_t), 1, fp);
  }
  fclose(fp);
}

// Creates a file and writes int values to it.
void write_file_int() {
  FILE *fp = fopen(filename, "w");
  for (int i = min_int; i < max_int; i += int_step) {
    fwrite(&i, sizeof(int), 1, fp);
  }
  fclose(fp);
}

// Tests

// Opens an existing file for writing
void test_open_existing_read() {
  create_file();

  bit_io_t *b = bit_io_open(filename, "r");
  TEST_ASSERT_NOT_NULL(b);
}

// Opens an existing file
void test_open_existing_write() {
  create_file();

  bit_io_t *b = bit_io_open(filename, "w");
  TEST_ASSERT_NOT_NULL(b);
}

// Opens a non-existent file
void test_open_non_existing_read() {
  bit_io_t *b = bit_io_open(filename, "r");
  TEST_ASSERT_NULL(b);
}

// Opens a file with an unsupported mode
void test_open_wrong_mode() {
  create_file();

  bit_io_t *b = bit_io_open(filename, "r+");
  TEST_ASSERT_NULL(b);
  b = bit_io_open(filename, "w+");
  TEST_ASSERT_NULL(b);
}

// Writes 1~8 bits to a file
void test_write_bits() {
  bit_io_t *b = bit_io_open(filename, "w");
  
  // Writes the following:
  //   0 in 1 bit
  //   1 in 2 bits
  //   2 in 3 bits
  //   3 in 4 bits
  //   ...
  //
  // Bits written
  // 0 01 010 0011 00100 000101 0000110 00000111
  //
  // Bytes written
  // 00101000 11001000 00101000 01100000 0111[0000] (final 4 zeros padded)
  // 0x28 0xC8 0x28 0x60 0x07

  int r = 0;
  for (uint8_t i = 0; i < 8; i++) {
    r = i + 1;
    bit_io_write_bits(b, i, r);
  }
  bit_io_close(b);

  // Read the data we just wrote, using standard C library
  // The pattern below results from the packed bits we wrote above
  uint8_t expected[] = {0x28, 0xC8, 0x28, 0x60, 0x70};
  FILE *fp = fopen(filename, "r");
  uint8_t data;
  for (uint8_t i = 0; i < 5; i++) {
    TEST_ASSERT_EQUAL(1, fread(&data, 1, 1, fp));
    TEST_ASSERT_EQUAL(expected[i], data);
  }
  fclose(fp);
}

// Reads 1~8 bits from a file
void test_read_bits() {
  // Write a packed bit test file
  FILE *fp = fopen(filename, "w");
  // Last value is 0x70 here because we do not automatically pack bits on a read
  uint8_t expected[] = {0x28, 0xC8, 0x28, 0x60, 0x70};
  for (int i = 0; i < 5; i++) {
    fwrite(&expected[i], 1, 1, fp);
  }
  fclose(fp);

  bit_io_t *b = bit_io_open(filename, "r");
  for (uint8_t i = 0; i < 8; i++) {
    TEST_ASSERT_EQUAL(i, bit_io_read_bits(b, i + 1));
  }
  bit_io_close(b);
}

// Writes single bit values to a file
void test_write_bit() {
  // Write data with bit_io
  bit_io_t *b = bit_io_open(filename, "w");
  for (uint8_t i = 0; i < 255; i++) {
    // Write MSB first
    for (int j = 7; j >= 0; j--) {
      bit_io_write_bit(b, (i & (1 << j)) > 0);
    }
  }
  bit_io_close(b);

  // Read the data we just wrote, using standard C library
  FILE *fp = fopen(filename, "r");
  uint8_t data;
  for (uint8_t i = 0; i < 255; i++) {
    TEST_ASSERT_EQUAL(1, fread(&data, 1, 1, fp));
    TEST_ASSERT_EQUAL(i, data);
  }
  fclose(fp);
}

// Reads single bit values from a file
void test_read_bit() {
  write_file_byte();

  // Read data with bit_io
  uint8_t data;
  bit_io_t *b = bit_io_open(filename, "r");
  for (uint8_t i = min_byte; i < max_byte; i+=byte_step) {
    data = 0;
    // Read MSB first
    for (int j = 7; j >= 0; j--) {
      data |= (bit_io_read_bit(b) << j);
    }
    TEST_ASSERT_EQUAL(i, data);
  }
  bit_io_close(b);
}

// Writes bytes to a file
void test_write_byte() {
  // Write data with bit_io
  bit_io_t *b = bit_io_open(filename, "w");
  for (uint8_t i = min_byte; i < max_byte; i+=byte_step) {
    bit_io_write_byte(b, i);
  }
  bit_io_close(b);

  // Read the data we just wrote, using standard C library
  FILE *fp = fopen(filename, "r");
  uint8_t data;
  for (uint8_t i = min_byte; i < max_byte; i+=byte_step) {
    TEST_ASSERT_EQUAL(1, fread(&data, sizeof(uint8_t), 1, fp));
    TEST_ASSERT_EQUAL(i, data);
  }
  fclose(fp);
}

// Reads bytes from a file
void test_read_byte() {
  write_file_byte();

  // Read data with bit_io
  uint8_t data;
  bit_io_t *b = bit_io_open(filename, "r");
  for (uint8_t i = min_byte; i < max_byte; i+=byte_step) {
    data = bit_io_read_byte(b);
    TEST_ASSERT_EQUAL(i, data);
  }
  bit_io_close(b);
}

// Writes ints to a file
void test_write_int() {
  write_file_int();

  bit_io_t *b = bit_io_open(filename, "r");
  for (int i = min_int; i < max_int; i+=int_step) {
    TEST_ASSERT_EQUAL(i, bit_io_read_int(b));
  }
  bit_io_close(b);
}

// Reads ints from a file
void test_read_int() {
  bit_io_t *b = bit_io_open(filename, "w");
  for (int i = min_int; i < max_int; i+=int_step) {
    bit_io_write_int(b, i);
  }
  bit_io_close(b);

  FILE *fp = fopen(filename, "r");
  int data;
  for (int i = min_int; i < max_int; i+=int_step) {
    TEST_ASSERT_EQUAL(1, fread(&data, sizeof(int), 1, fp));
    TEST_ASSERT_EQUAL(i, data);
  }
  fclose(fp);
}

// Writes different bit lengths to a file
void test_write_interleaved() {
  bit_io_t *b = bit_io_open(filename, "w");

  bit_io_write_bit(b, true);
  bit_io_write_bits(b, 0xFA, 6);
  bit_io_write_byte(b, 0xCE);
  bit_io_write_int(b, 0xDEADBEEF);

  bit_io_close(b);
  
  // Bits written
  // 1 111010 11001110 11101111 10111110 10101101 11011110
  // Bytes written  (last byte is padded)
  // 11110101 10011101 11011111 01111101 01011011 1011110[0]
  // f5 9d df 7d 5b 5e

  FILE *fp = fopen(filename, "r");
  uint8_t data;
  uint8_t expected[] = {0xf5, 0x9d, 0xdf, 0x7d, 0x5b, 0xbc};
  for (int i = 0; i < 6; i++) {
    TEST_ASSERT_EQUAL(1, fread(&data, 1, 1, fp));
    TEST_ASSERT_EQUAL(expected[i], data);
  }
  fclose(fp);
}

// Reads different bit lengths from a file
void test_read_interleaved() {
  FILE *fp = fopen(filename, "w");
  // last byte shifted from write_interleaved test to account for padding
  uint8_t expected[] = {0xf5, 0x9d, 0xdf, 0x7d, 0x5b, 0xbc};
  for (int i = 0; i < 6; i++) {
    fwrite(&expected[i], 1, 1, fp);
  }
  fclose(fp);

  bit_io_t *b = bit_io_open(filename, "r");

  TEST_ASSERT_TRUE(bit_io_read_bit(b));
  TEST_ASSERT_EQUAL(0x3a, bit_io_read_bits(b, 6));
  TEST_ASSERT_EQUAL(0xce, bit_io_read_byte(b));
  TEST_ASSERT_EQUAL_INT32(0xdeadbeef, bit_io_read_int(b));

  bit_io_close(b);
}

// Checks EOF indicator upon reading a file
void test_eof_on_read() {
  write_file_byte();

  bit_io_t *b = bit_io_open(filename, "r");
  for (int i = min_byte; i < max_byte; i++) {
    TEST_ASSERT_FALSE(bit_io_eof(b));
    TEST_ASSERT_EQUAL(i, bit_io_read_byte(b));
  }
  
  TEST_ASSERT_TRUE(bit_io_eof(b));
  bit_io_close(b);
}

// Checks EOF indicator upon writing a file
void test_eof_on_write() {
  bit_io_t *b = bit_io_open(filename, "w");
  TEST_ASSERT_FALSE(bit_io_eof(b));
  bit_io_close(b);
}

// Tries to write more than 8 bits
void test_write_too_many_bits() {
  bit_io_t *b = bit_io_open(filename, "w");
  bit_io_write_bits(b, 255, 9);
}

// Tries to write 0 bits
void test_write_too_few_bits() {
  bit_io_t *b = bit_io_open(filename, "w");
  bit_io_write_bits(b, 255, 0);
}

// Tries to read more than 8 bits
void test_read_too_many_bits() {
  write_file_byte();

  bit_io_t *b = bit_io_open(filename, "r");
  bit_io_write_bits(b, 255, 9);
}

// Tries to read 0 bits
void test_read_too_few_bits() {
  write_file_byte();

  bit_io_t *b = bit_io_open(filename, "r");
  bit_io_write_bits(b, 255, 0);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_open_existing_read);
  RUN_TEST(test_open_existing_write);
  RUN_TEST(test_open_non_existing_read);
  RUN_TEST(test_open_wrong_mode);
  
  RUN_TEST(test_write_bit);
  RUN_TEST(test_read_bit);

  RUN_TEST(test_write_bits);
  RUN_TEST(test_read_bits);

  RUN_TEST(test_write_byte);
  RUN_TEST(test_read_byte);
  
  RUN_TEST(test_write_int);
  RUN_TEST(test_read_int);
  
  RUN_TEST(test_write_interleaved);
  RUN_TEST(test_read_interleaved);

  RUN_TEST(test_eof_on_write);
  RUN_TEST(test_eof_on_read);

  RUN_TEST(test_write_too_many_bits);
  RUN_TEST(test_write_too_few_bits);
  RUN_TEST(test_read_too_many_bits);
  RUN_TEST(test_read_too_few_bits);

  return UNITY_END();
}
