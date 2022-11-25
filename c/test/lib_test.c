#include "../../../Unity/src/unity.h"
#include "../lib/lib.h"
#include <string.h>
#include <stdlib.h>

void setUp() {
}

void tearDown() {
}

void test_nelems() {
  int a[5];
  TEST_ASSERT_EQUAL(5, NELEMS(a));

  char b[9182];
  TEST_ASSERT_EQUAL(9182, NELEMS(b));
}

void test_less_int() {
  int a[] = {4, -5, 948, 0, -5, 0, 5, 4};
  int b[] = {5, 5, 5000, 5, -5, 0, 0, 1};
  bool result[] = {true, true, true, true, false, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_int(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_int(&a[i], &b[i]));
    }
  }
}

void test_compare_int() {
  int a[] = {4, -5, 948, 0, -5, 0, 5, 4};
  int b[] = {5, 5, 5000, 5, -5, 0, 0, 1};
  int result[] = {-1, -1, -1, -1, 0, 0, 1, 1};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_int(&a[i], &b[i]));
  }
}

void test_less_uint() {
  unsigned a[] = {4, 948,  0, 0, 5, 4};
  unsigned b[] = {5, 5000, 5, 0, 0, 1};
  bool result[] = {true, true, true, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_uint(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_uint(&a[i], &b[i]));
    }
  }
}

void test_compare_uint() {
  unsigned a[] = {4, 948,  0, 0, 5, 4};
  unsigned b[] = {5, 5000, 5, 0, 0, 1};
  int result[] = {-1, -1, -1, 0, 1, 1};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_uint(&a[i], &b[i]));
  }
}

void test_less_float() {
  float a[] = {4.5, -5.0, 948.123,  0,   -5.6, 0, 5.2, 4.1};
  float b[] = {5.1,  5.9, 5000.345, 5.2, -5.6, 0, 0.0, 1.1};
  bool result[] = {true, true, true, true, false, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_float(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_float(&a[i], &b[i]));
    }
  }
}

void test_compare_float() {
  float a[] = {4.5, -5.0, 948.123,  0,   -5.6, 0, 5.2, 4.1};
  float b[] = {5.1,  5.9, 5000.345, 5.2, -5.6, 0, 0.0, 1.1};
  int result[] = {-1, -1, -1, -1, 0, 0, 1, 1};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_float(&a[i], &b[i]));
  }
}

void test_less_double() {
  double a[] = {4.5, -5.0, 948.123,  0,   -5.6, 0, 5.2, 4.1};
  double b[] = {5.1,  5.9, 5000.345, 5.2, -5.6, 0, 0.0, 1.1};
  bool result[] = {true, true, true, true, false, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_double(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_double(&a[i], &b[i]));
    }
  }
}

void test_compare_double() {
  double a[] = {4.5, -5.0, 948.123,  0,   -5.6, 0, 5.2, 4.1};
  double b[] = {5.1,  5.9, 5000.345, 5.2, -5.6, 0, 0.0, 1.1};
  int result[] = {-1, -1, -1, -1, 0, 0, 1, 1};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_double(&a[i], &b[i]));
  }
}

void test_less_char() {
  char a[] = "addxf";
  char b[] = "bzdae";
  bool result[] = {true, true, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_char(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_char(&a[i], &b[i]));
    }
  }
}

void test_compare_char() {
  char a[5] = "addxf";
  char b[5] = "bzdae";
  int result[] = {-1, -1, 0, 1, 1};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_char(&a[i], &b[i]));
  }
}

void test_less_str() {
  char *a[] = {"hello", "h", "hello", "world", ""};
  char *b[] = {"world", "w", "hello", "hello", ""};
  bool result[] = {true, true, false, false, false};

  for (int i = 0; i < NELEMS(a); i++) {
    if (result[i]) {
      TEST_ASSERT_TRUE(less_str(&a[i], &b[i]));
    } else {
      TEST_ASSERT_FALSE(less_str(&a[i], &b[i]));
    }
  }
}

void test_compare_str() {
  char *a[] = {"hello", "h", "hello", "world", ""};
  char *b[] = {"world", "w", "hello", "hello", ""};
  int result[] = {-1, -1, 0, 1, 0};

  for (int i = 0; i < NELEMS(a); i++) {
    TEST_ASSERT_EQUAL(result[i], compare_str(&a[i], &b[i]));
  }
}

void test_hash() {
  char *alphanum = "abcdefghijklmnopqrstuvwxyz0123456789";
  uint32_t hash;
  int mod = 37;    // modulo for hash output, prime
  int counts[mod];  // count hash occurrences

  for (int i = 0; i < mod; i++) {
    counts[i] = 0;
  }

  for (int i = 0; i < strlen(alphanum); i++) {
    hash = fnv_hash_32(&alphanum[i], sizeof(char)) % mod;
    counts[hash]++;
  }

  for (int i = 0; i < mod; i++) {
    // printf("%d : %d\n", i, counts[i]);
    TEST_ASSERT_LESS_OR_EQUAL(2, counts[i]);
  }
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_nelems);
  RUN_TEST(test_less_int);
  RUN_TEST(test_compare_int);
  RUN_TEST(test_less_uint);
  RUN_TEST(test_compare_uint);
  RUN_TEST(test_less_float);
  RUN_TEST(test_compare_float);
  RUN_TEST(test_less_double);
  RUN_TEST(test_compare_double);
  RUN_TEST(test_less_char);
  RUN_TEST(test_compare_char);
  RUN_TEST(test_less_str);
  RUN_TEST(test_compare_str);
  RUN_TEST(test_hash);
  return UNITY_END();
}
