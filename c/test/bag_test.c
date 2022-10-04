#include "../../../Unity/src/unity.h"
#include "../bag/bag.h"
#include <string.h>

static int values[6] = {10, 3, 4, -11, 0, 999};
static char *strs[6] = {"Hello", "world!", "my", "name", "is", "Beelzebub!"};
static bag_t *b;

void setUp() {
}

void tearDown() {
}

// Helper functions
void fill_ints() {
  b = bag_init(sizeof(int));
  for (int i = 0; i < NELEMS(values); i++) {
    bag_add(b, &values[i]);
  }
}

void fill_strs() {
  b = bag_init(sizeof(char *));
  char *str;
  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    bag_add(b, &str);
  }
}

// Tests

// Tests that a new bag is empty
void test_new_bag_is_empty() {
  b = bag_init(sizeof(int));

  TEST_ASSERT_NOT_NULL(b);
  TEST_ASSERT_EQUAL(0, bag_size(b));
}

// Tests that a bag has the correct number of ints
void test_add_ints() {
  fill_ints();
  TEST_ASSERT_EQUAL(NELEMS(values), bag_size(b));
}

// Tests that a bag of ints can be iterated over and that its contents match the
// reverse order of insertion
void test_iter_ints_reverse_order() {
  int value, counter;
  unsigned int size = bag_size(b);
  counter = 1;

  fill_ints();
  bag_iter_init(b);
  while (bag_iter_has_next(b)) {
    bag_iter_next(b, &value);
    TEST_ASSERT_EQUAL(value, values[size - counter]);
    counter++;
  }
}

// Tests that a bag has the correct number of strings
void test_add_strs() {
  fill_strs();
  TEST_ASSERT_EQUAL(NELEMS(strs), bag_size(b));
}

// Tests that a bag of strings can be iterated over and that its contents match
// the reverse order of insertion
void test_iter_strs_reverse_order() {
  int counter;
  unsigned int size = bag_size(b);
  char *str;
  counter = 1;

  fill_strs();
  bag_iter_init(b);
  while (bag_iter_has_next(b)) {
    bag_iter_next(b, &str);
    TEST_ASSERT_EQUAL_STRING(str, strs[size - counter]);
    counter++;
  }
}

// Tests creating a bag of ints and freeing it
void test_int_free() {
  fill_ints();
  bag_free(b);
  TEST_ASSERT_NULL(b);
}

// Tests creating a bag of strs and freeing it
void test_str_free() {
  fill_strs();
  bag_free(b);
  TEST_ASSERT_NULL(b);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_new_bag_is_empty);
  RUN_TEST(test_add_ints);
  RUN_TEST(test_add_strs);
  RUN_TEST(test_iter_ints_reverse_order);
  RUN_TEST(test_iter_strs_reverse_order);
  RUN_TEST(test_int_free);
  RUN_TEST(test_str_free);
  return UNITY_END();
}
