#include "../../../Unity/src/unity.h"
#include "../queue/queue.h"
#include <string.h>

static int values[6] = {10, 3, 4, -11, 0, 999};
static char *strs[6] = {"Hello", "world!", "my", "name", "is", "Beelzebub!"};
static queue_t *q;

void setUp() {
}

void tearDown() {
}

// Helper functions
void fill_ints() {
  q = queue_init(sizeof(int));
  for (int i = 0; i < NELEMS(values); i++) {
    queue_enqueue(q, &values[i]);
  }
}

void fill_strs() {
  q = queue_init(sizeof(char *));
  char *str;
  for (int i = 0; i < NELEMS(strs); i++) {
    str = strdup(strs[i]);
    queue_enqueue(q, &str);
  }
}

// Tests

// Tests that a new queue is empty
void test_new_queue_is_empty() {
  q = queue_init(sizeof(int));

  TEST_ASSERT_NOT_NULL(q);
  TEST_ASSERT_EQUAL(0, queue_size(q));
  TEST_ASSERT_TRUE(queue_is_empty(q));
}

// Tests that a queue has the correct number of ints
void test_add_ints() {
  fill_ints();
  TEST_ASSERT_EQUAL(NELEMS(values), queue_size(q));
}

// Tests that a queue of ints can be iterated over and that its contents match the
// order of insertion
void test_iter_ints_forward_order() {
  fill_ints();

  int value, counter;
  counter = 0;

  queue_iter_init(q);
  while (queue_iter_has_next(q)) {
    queue_iter_next(q, &value);
    TEST_ASSERT_EQUAL(value, values[counter]);
    counter++;
  }
}

// Tests that a queue of ints dequeues values in insertion order, and that
// the queue is empty after the last item is dequeued
void test_dequeue_ints() {
  fill_ints();

  int value;
  int counter;
  counter = 0;

  while (!queue_is_empty(q)) {
    TEST_ASSERT_TRUE(queue_dequeue(q, &value));
    TEST_ASSERT_EQUAL(value, values[counter]);
    counter++;
  }
  TEST_ASSERT_EQUAL(0, queue_size(q));
}

// Tests dequeuing an empty queue of ints
void test_dequeue_empty_ints() {
  fill_ints();

  int value;
  while (!queue_is_empty(q)) {
    queue_dequeue(q, &value);
  }
  TEST_ASSERT_FALSE(queue_dequeue(q, &value));
}

// Tests that a queue has the correct number of strings
void test_add_strs() {
  fill_strs();
  TEST_ASSERT_EQUAL(NELEMS(strs), queue_size(q));
}

// Tests that a queue of strings can be iterated over and that its contents match
// the order of insertion
void test_iter_strs_forward_order() {
  fill_strs();

  int counter;
  char *str;
  counter = 0;

  queue_iter_init(q);
  while (queue_iter_has_next(q)) {
    queue_iter_next(q, &str);
    TEST_ASSERT_EQUAL_STRING(str, strs[counter]);
    counter++;
  }
}

// Tests that a queue of strs dequeues values in insertion order, and that
// the queue is empty after the last item is dequeued
void test_dequeue_strs() {
  fill_strs();

  char *str;
  int counter;
  counter = 0;

  while (!queue_is_empty(q)) {
    TEST_ASSERT_TRUE(queue_dequeue(q, &str));
    TEST_ASSERT_EQUAL_STRING(str, strs[counter]);
    counter++;
  }
  TEST_ASSERT_EQUAL(0, queue_size(q));
}

// Tests dequeueing an empty queue of strs
void test_dequeue_empty_strs() {
  fill_strs();

  char *str;
  while (!queue_is_empty(q)) {
    queue_dequeue(q, &str);
  }
  TEST_ASSERT_FALSE(queue_dequeue(q, &str));
}

// Tests creating a queue of ints and freeing it
void test_int_free() {
  fill_ints();
  queue_free(q);
}

// Tests creating a queue of strs and freeing it
void test_str_free() {
  fill_strs();
  queue_free(q);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_new_queue_is_empty);
  RUN_TEST(test_add_ints);
  RUN_TEST(test_add_strs);
  RUN_TEST(test_iter_ints_forward_order);
  RUN_TEST(test_iter_strs_forward_order);
  RUN_TEST(test_int_free);
  RUN_TEST(test_str_free);
  RUN_TEST(test_dequeue_ints);
  RUN_TEST(test_dequeue_strs);
  RUN_TEST(test_dequeue_empty_ints);
  RUN_TEST(test_dequeue_empty_strs);
  return UNITY_END();
}
