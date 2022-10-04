#include "../../../Unity/src/unity.h"
#include "../stack/stack.h"
#include <string.h>

static int values[6] = {10, 3, 4, -11, 0, 999};
static char *strs[6] = {"Hello", "world!", "my", "name", "is", "Beelzebub!"};
static my_stack_t *s;

void setUp() {
}

void tearDown() {
}

// Helper functions
void fill_ints() {
  s = stack_init(sizeof(int));
  for (int i = 0; i < NELEMS(values); i++) {
    stack_push(s, &values[i]);
  }
}

void fill_strs() {
  s = stack_init(sizeof(char *));
  char *str;
  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    stack_push(s, &str);
  }
}

// Tests

// Tests that a new stack is empty
void test_new_stack_is_empty() {
  s = stack_init(sizeof(int));

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL(0, stack_size(s));
  TEST_ASSERT_TRUE(stack_is_empty(s));
}

// Tests that a stack has the correct number of ints
void test_add_ints() {
  fill_ints();
  TEST_ASSERT_EQUAL(NELEMS(values), stack_size(s));
}

// Tests that a stack of ints can be iterated over and that its contents match the
// reverse order of insertion
void test_iter_ints_reverse_order() {
  fill_ints();

  int value, counter;
  unsigned int size = stack_size(s);
  counter = 1;

  stack_iter_init(s);
  while (stack_iter_has_next(s)) {
    stack_iter_next(s, &value);
    TEST_ASSERT_EQUAL(value, values[size - counter]);
    counter++;
  }
}

// Tests that a stack of ints pops values in reverse insertion order, and that
// the stack is empty after the last item is popped
void test_pop_ints() {
  fill_ints();

  int value;
  int counter;
  unsigned int size = stack_size(s);
  counter = 1;

  while (!stack_is_empty(s)) {
    TEST_ASSERT_TRUE(stack_pop(s, &value));
    TEST_ASSERT_EQUAL(value, values[size - counter]);
    counter++;
  }
  TEST_ASSERT_EQUAL(0, stack_size(s));
}

// Tests popping an empty stack of ints
void test_pop_empty_ints() {
  fill_ints();
  int value;
  while (!stack_is_empty(s)) {
    stack_pop(s, &value);
  }
  TEST_ASSERT_FALSE(stack_pop(s, &value));
}

// Tests that a stack has the correct number of strings
void test_add_strs() {
  fill_strs();
  TEST_ASSERT_EQUAL(NELEMS(strs), stack_size(s));
}

// Tests that a stack of strings can be iterated over and that its contents match
// the reverse order of insertion
void test_iter_strs_reverse_order() {
  fill_strs();

  int counter;
  unsigned int size = stack_size(s);
  char *str;
  counter = 1;

  stack_iter_init(s);
  while (stack_iter_has_next(s)) {
    stack_iter_next(s, &str);
    TEST_ASSERT_EQUAL_STRING(str, strs[size - counter]);
    counter++;
  }
}

// Tests that a stack of strs pops values in reverse insertion order, and that
// the stack is empty after the last item is popped
void test_pop_strs() {
  fill_strs();

  char *str;
  int counter;
  unsigned int size = stack_size(s);
  counter = 1;

  while (!stack_is_empty(s)) {
    TEST_ASSERT_TRUE(stack_pop(s, &str));
    TEST_ASSERT_EQUAL_STRING(str, strs[size - counter]);
    counter++;
  }
  TEST_ASSERT_EQUAL(0, stack_size(s));
}

// Tests popping an empty stack of strs
void test_pop_empty_strs() {
  fill_strs();
  char *str;
  while (!stack_is_empty(s)) {
    stack_pop(s, &str);
  }
  TEST_ASSERT_FALSE(stack_pop(s, &str));
}

// Tests creating a stack of ints and freeing it
void test_int_free() {
  fill_ints();
  stack_free(s);
}

// Tests creating a stack of strs and freeing it
void test_str_free() {
  fill_strs();
  stack_free(s);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_new_stack_is_empty);
  RUN_TEST(test_add_ints);
  RUN_TEST(test_add_strs);
  RUN_TEST(test_iter_ints_reverse_order);
  RUN_TEST(test_iter_strs_reverse_order);
  RUN_TEST(test_int_free);
  RUN_TEST(test_str_free);
  RUN_TEST(test_pop_ints);
  RUN_TEST(test_pop_strs);
  RUN_TEST(test_pop_empty_ints);
  RUN_TEST(test_pop_empty_strs);
  return UNITY_END();
}
