#include "../../../Unity/src/unity.h"
#include "../symbol_table/symbol_table.h"
#include <string.h>
#include <stdlib.h>

static char *orig_str[] = {"she", "sells", "sea", "shells", "by", "the", "shore", "shell", "shellsort", "shelters"};
static char *prefix_she[] = {"she", "shell", "shells", "shellsort", "shelters"};
static char *prefix_se[] = {"sea", "sells"};
static char *match_dothe[] = {"she", "the"};
static char *match_sdotdot[] = {"sea", "she"};
static int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

st_t *st;
int st_type;
int st_types[] = {TRIE_RWAY};

void setUp() {
  st = st_init(sizeof(char *), sizeof(int), compare_str, st_type);
}

void tearDown() {
  free(st);
}

// Helper functions, fills symbol table with ascii value for the inserted chars.
void fill_st() {
  for (int i = 0; i < NELEMS(orig_str); i++) {
    st_put(st, &orig_str[i], &values[i]);
  }
}

// Tests

// Create a symbol table used to store character keys and integer values and
// check that the size and empty functions report the correct values.
void test_new_st_is_empty() {
  TEST_ASSERT_NOT_NULL(st);
  TEST_ASSERT_EQUAL(0, st_size(st));
  TEST_ASSERT_TRUE(st_is_empty(st));
}

// Fill the symbol table and check that it has the correct number of keys.
void test_st_put() {
  fill_st();

  TEST_ASSERT_EQUAL(NELEMS(orig_str), st_size(st));
  char *key = "hello";
  int value = 0;
  TEST_ASSERT_TRUE(st_put(st, &key, &value)); // check return value
}

// Try to add null keys/values
void test_st_put_null() {
  char *key = "hello";
  int value;
  
  value = 0;
  TEST_ASSERT_FALSE(st_put(st, NULL, &value));
  TEST_ASSERT_FALSE(st_put(st, &key, NULL));
}

// Check values inserted into the symbol table.
void test_st_get() {
  fill_st();

  int value;
  for (int i = 0; i < NELEMS(orig_str); i++) {
    TEST_ASSERT_TRUE(st_get(st, &orig_str[i], &value));
    TEST_ASSERT_EQUAL(values[i], value);
    TEST_ASSERT_TRUE(st_contains(st, &orig_str[i]));
  }
}

// Check get with a missing key
void test_st_get_missing() {
  fill_st();

  int value;
  char *key = "goodbye";
  TEST_ASSERT_FALSE(st_get(st, &key, &value));
  TEST_ASSERT_FALSE(st_contains(st, &key));
}

// Test put/get with duplicated keys that increment a count
void test_st_count() {
  int count;
  fill_st();

  for (int i = 0; i < NELEMS(orig_str); i++) {
    st_get(st, &orig_str[i], &count);
    count++;
    st_put(st, &orig_str[i], &count);
  }
  for (int i = 0; i < NELEMS(orig_str); i++) {
    TEST_ASSERT_TRUE(st_get(st, &orig_str[i], &count));
    TEST_ASSERT_EQUAL(values[i] + 1, count);
  }
}

// Test iteration on an empty symbol table
void test_st_iter_empty() {
  char *key;
  int value;
  int counter = 0;

  st_iter_init(st);
  while (st_iter_has_next(st)) {
    st_iter_next(st, &key);
    TEST_ASSERT_TRUE(st_get(st, &key, &value));
    counter++;
  }
  TEST_ASSERT_EQUAL(0, counter);
}

// Test iteration on a full symbol table
void test_st_iter() {
  char *key;
  int value;
  int counter = 0;

  fill_st();
  st_iter_init(st);
  while (st_iter_has_next(st)) {
    st_iter_next(st, &key);
    // st_get(st, &key, &value);
    TEST_ASSERT_TRUE(st_get(st, &key, &value));
    TEST_ASSERT_EQUAL_STRING(key, orig_str[value]);
    // printf("key: %s, value: %d\n", key, value);
    counter++;
  }
  TEST_ASSERT_EQUAL(st_size(st), counter);
}

void test_st_longest_prefix() {
  fill_st();

  char *key;
  TEST_ASSERT_TRUE(st_longest_prefix_of(st, "shell", &key));
  TEST_ASSERT_EQUAL_STRING("she", key);

  TEST_ASSERT_TRUE(st_longest_prefix_of(st, "shells", &key));
  TEST_ASSERT_EQUAL_STRING("shell", key);
}

void test_st_keys_with_prefix() {
  fill_st();

  char *key;
  char *prefix1 = "she";
  st_prefix_iter_init(st, prefix1);

  int i = 0;
  while (st_prefix_iter_has_next(st)) {
    st_prefix_iter_next(st, &key);
    TEST_ASSERT_EQUAL_STRING(prefix_she[i], key);
    i++;
    // printf("prefix: %s, key: %s\n", prefix1, key);
  }

  char *prefix2 = "se";
  st_prefix_iter_init(st, prefix2);

  i = 0;
  TEST_ASSERT_TRUE(st_prefix_iter_has_next(st));
  while (st_prefix_iter_has_next(st)) {
    st_prefix_iter_next(st, &key);
    TEST_ASSERT_EQUAL_STRING(prefix_se[i], key);
    i++;
    // printf("prefix: %s, key: %s\n", prefix2, key);
  }
}

void test_st_keys_that_match() {
  fill_st();
  
  char *key;
  char *match1 = ".he";
  st_match_iter_init(st, match1);
  TEST_ASSERT_TRUE(st_match_iter_has_next(st));
  
  int i = 0;
  while (st_match_iter_has_next(st)) {
    st_match_iter_next(st, &key);
    TEST_ASSERT_EQUAL_STRING(match_dothe[i], key);
    i++;
    // printf("match: %s, key: %s\n", match1, key);
  }

  char *match2 = "s..";
  st_match_iter_init(st, match2);
  TEST_ASSERT_TRUE(st_match_iter_has_next(st));
  
  i = 0;
  while (st_match_iter_has_next(st)) {
    st_match_iter_next(st, &key);
    TEST_ASSERT_EQUAL_STRING(match_sdotdot[i], key);
    i++;
    // printf("match: %s, key: %s\n", match2, key);
  }
}

// Main
int main() {
  UNITY_BEGIN();
  for (int i = 0; i < NELEMS(st_types); i++) {
    st_type = st_types[i];
    RUN_TEST(test_new_st_is_empty);
    RUN_TEST(test_st_put);
    RUN_TEST(test_st_put_null);
    RUN_TEST(test_st_get);
    RUN_TEST(test_st_get_missing);
    RUN_TEST(test_st_count);
    RUN_TEST(test_st_iter_empty);
    RUN_TEST(test_st_iter);
    RUN_TEST(test_st_longest_prefix);
    RUN_TEST(test_st_keys_with_prefix);
    RUN_TEST(test_st_keys_that_match);
  }
  return UNITY_END();
}
