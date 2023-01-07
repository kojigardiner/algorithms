#include "../../../Unity/src/unity.h"
#include "../zip/zip.h"
#include <stdlib.h>

static const char *filenames[] = {"../data/tinyTale.txt", "../data/tale.txt", "../data/q64x96.bin", "../data/abra.txt", "../data/4runs.bin"};
static const char *compressed_filename = "compressed";
static const char *expanded_filename = "expanded";

static int types[] = {RLE, HUFFMAN, LZW};
static char *type_names[] = {"RLE", "HUFFMAN", "LZW"};

static const char *filename;
static const char *type_name;
static int type;

void setUp() {
  // remove(compressed_filename);
  // remove(expanded_filename);
}

void tearDown() {
}

// Tests
void test_verify_true() {
  TEST_ASSERT_TRUE(verify(filenames[1], filenames[1]));
}

void test_verify_false() {
  TEST_ASSERT_FALSE(verify(filenames[0], filenames[2]));
}

void test_zip() {
  double ratio = compress(filename, compressed_filename, type);
  expand(compressed_filename, expanded_filename, type);
  printf("%24s %10s: %.2f %%\n", filename, type_name, ratio * 100);
  TEST_ASSERT_TRUE(verify(filename, expanded_filename));
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_verify_true);
  RUN_TEST(test_verify_false);
  for (int i = 0; i < NELEMS(filenames); i++) {
    filename = filenames[i];
    for (int j = 0; j < 1; j++) {//NELEMS(types); j++) {
      type = types[j];
      type_name = type_names[j];
      RUN_TEST(test_zip);
    }
  }
  return UNITY_END();
}
