#include "../../../Unity/src/unity.h"
#include "../lib/lib.h"
#include <string.h>

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

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_nelems);
  RUN_TEST(test_less_int);
  RUN_TEST(test_less_uint);
  RUN_TEST(test_less_float);
  RUN_TEST(test_less_double);
  RUN_TEST(test_less_char);
  RUN_TEST(test_less_str);
  return UNITY_END();
}
