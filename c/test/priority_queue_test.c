#include "../../../Unity/src/unity.h"
#include "../priority_queue/priority_queue.h"
#include <string.h>
#include <stdlib.h>

static char *orig_str = "thequickbrownfoxjumpsoverthelazydog";
static char *min_str = "abcdeeefghhijklmnoooopqrrsttuuvwxyz";
static char *max_str = "zyxwvuuttsrrqpoooonmlkjihhgfeeedcba";

pq_t *pq;

void setUp() {
}

void tearDown() {
}

// Helper functions
void fill_max_chars() {
  size_t max = strlen(orig_str);
  pq = pq_init(sizeof(char), max, MAX, less_char);
  for (int i = 0; i < max; i++) {
    pq_insert(pq, &orig_str[i]);
  }
}

void fill_min_chars() {
  size_t max = strlen(orig_str);
  pq = pq_init(sizeof(char), max, MIN, less_char);
  for (int i = 0; i < max; i++) {
    pq_insert(pq, &orig_str[i]);
  }
}

// Tests

// Tests initializing a priority queue for characters
void test_max_pq_init() {
  size_t max = 100;
  pq = pq_init(sizeof(char), max, MAX, less_char);
  TEST_ASSERT_NOT_NULL(pq);
  TEST_ASSERT_TRUE(pq_is_empty(pq));
  TEST_ASSERT_EQUAL(0, pq_size(pq));
}

// Tests a priority queue: insertion, max, and removal of characters
void test_max_pq_char() {
  int i;
  size_t max = strlen(orig_str);
  fill_max_chars();
  TEST_ASSERT_EQUAL(max, pq_size(pq));

  char c;
  i = 0;
  while (!pq_is_empty(pq)) {
    // Check that max works
    pq_front(pq, &c);
    TEST_ASSERT_EQUAL(max - i, pq_size(pq));
    TEST_ASSERT_EQUAL_CHAR(max_str[i], c);
    
    // Check that del works
    c = ' ';
    pq_del(pq, &c);
    TEST_ASSERT_EQUAL_CHAR(max_str[i], c);
    TEST_ASSERT_EQUAL(max - i - 1, pq_size(pq));
    i++;
  }
}

// Tests a MIN priority queue: insertion, max, and removal of characters
void test_min_pq_char() {
  int i;
  size_t max = strlen(orig_str);
  fill_min_chars();
  TEST_ASSERT_EQUAL(max, pq_size(pq));

  char c;
  i = 0;
  while (!pq_is_empty(pq)) {
    // Check that max works
    pq_front(pq, &c);
    TEST_ASSERT_EQUAL(max - i, pq_size(pq));
    TEST_ASSERT_EQUAL_CHAR(min_str[i], c);
    
    // Check that del works
    c = ' ';
    pq_del(pq, &c);
    TEST_ASSERT_EQUAL_CHAR(min_str[i], c);
    TEST_ASSERT_EQUAL(max - i - 1, pq_size(pq));
    i++;
  }
}

void test_max_pq_insert_on_full() {
  fill_max_chars();
  char c = 'a';
  TEST_ASSERT_FALSE(pq_insert(pq, &c));
}

void test_max_pq_del_on_empty() {
  size_t max = strlen(orig_str);
  fill_max_chars();
  char c;
  for (int i = 0; i < max; i++) {
    TEST_ASSERT_TRUE(pq_del(pq, &c));
  }
  TEST_ASSERT_FALSE(pq_del(pq, &c));
}

void test_max_pq_free() {
  fill_max_chars();
  pq_free(pq);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_max_pq_init);
  RUN_TEST(test_max_pq_char);
  RUN_TEST(test_max_pq_insert_on_full);
  RUN_TEST(test_max_pq_del_on_empty);
  RUN_TEST(test_max_pq_free);
  RUN_TEST(test_min_pq_char);
  return UNITY_END();
}
