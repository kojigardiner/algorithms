#include "../../../Unity/src/unity.h"
#include "../symbol_table/symbol_table.h"
#include <string.h>
#include <stdlib.h>

static char *orig_str = "thequickbrownfoxjumpsoverthelazydog";
static int counts[] = {1,1,1,1,3,1,1,2,1,1,1,1,1,1,4,1,1,2,1,2,2,1,1,1,1,1};
static int alphabet_len = (int)('z' - 'a' + 1);
st_t *st;
int st_type;
int st_types[] = {BST, RED_BLACK_BST, SEQUENTIAL_SEARCH, HASH_TABLE_CHAINING, HASH_TABLE_PROBING};
int trie_st_types[] = {TRIE_RWAY, TRIE_TST};

void setUp() {
  st = st_init(sizeof(char), sizeof(int), compare_char, st_type);
}

void tearDown() {
  free(st);
}

// Helper functions, fills symbol table with ascii value for the inserted chars.
void fill_st() {
  for (int i = 0; i < strlen(orig_str); i++) {
    int value = orig_str[i];
    st_put(st, &orig_str[i], &value);
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

  TEST_ASSERT_EQUAL(alphabet_len, st_size(st));
  char key = 'a';
  int value = 0;
  TEST_ASSERT_TRUE(st_put(st, &key, &value)); // check return value
}

// Try to add null keys/values
void test_st_put_null() {
  char key;
  int value;
  
  value = 0;
  TEST_ASSERT_FALSE(st_put(st, NULL, &value));
  key = 'a';
  TEST_ASSERT_FALSE(st_put(st, &key, NULL));
}

// Check values inserted into the symbol table.
void test_st_get() {
  fill_st();

  int ascii;
  for (char i = 'a'; i <= 'z'; i++) {
    TEST_ASSERT_TRUE(st_get(st, &i, &ascii));
    TEST_ASSERT_EQUAL((int)i, ascii);
    TEST_ASSERT_TRUE(st_contains(st, &i));
  }
}

// Check get with a missing key
void test_st_get_missing() {
  fill_st();

  int ascii;
  char key = '1';
  TEST_ASSERT_FALSE(st_get(st, &key, &ascii));
  TEST_ASSERT_FALSE(st_contains(st, &key));
}

// Test put/get with duplicated keys that increment a count
void test_st_count() {
  int count;
  for (int i = 0; i < strlen(orig_str); i++) {
    count = 0;
    st_get(st, &orig_str[i], &count);
    count++;
    st_put(st, &orig_str[i], &count);
  }
  for (char i = 'a'; i <= 'z'; i++) {
    TEST_ASSERT_TRUE(st_get(st, &i, &count));
    TEST_ASSERT_EQUAL(counts[(int)(i - 'a')], count);
  }
}

// Test iteration on an empty symbol table
void test_st_iter_empty() {
  char key;
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
  char key;
  int value;
  int counter = 0;

  fill_st();
  st_iter_init(st);
  while (st_iter_has_next(st)) {
    st_iter_next(st, &key);
    TEST_ASSERT_TRUE(st_get(st, &key, &value));
    TEST_ASSERT_EQUAL((int)(key), value);
    // printf("key: %c, value: %d\n", key, value);
    counter++;
  }
  TEST_ASSERT_EQUAL(st_size(st), counter);
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
  }
  return UNITY_END();
}
