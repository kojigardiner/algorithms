#include "../../../Unity/src/unity.h"
#include "../sort/sort.h"
#include <string.h>
#include <stdlib.h>

#define RAND_SEED 0

// Initially sorted arrays as input
typedef struct custom {
  char name[16];
  int id;
} custom_t;

static const int count = 10;
static const int str_count = 13;
static int int_arr[count];
static int int_dup_arr[count];
static float float_arr[count];
static double double_arr[count];
static unsigned int uint_arr[count];
static char *str_arr[count];
static char *str_arr2[str_count];
static char *str_arr3[str_count];
static char char_arr[count];
static int permutation[count];
custom_t custom_arr[count];
enum sort_type types[7] = {SELECTION, INSERTION, SHELL, MERGE_TD, MERGE_BU, QUICK, HEAP};
enum sort_type str_sort_types[3] = {STR_LSD, STR_MSD, STR_THREEWAY_QUICK};

void setUp() {
  srand(RAND_SEED);   // seed PRNG

  // Reset global data
  int int_data[count] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
  int int_dup_data[count] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  float float_data[count] = {0.1, 1.2, 2.4, 3.3, 4.7, 5.3, 6.1, 7.9, 8.1, 9.8};
  double double_data[count] = {0.1, 1.2, 2.4, 3.3, 4.7, 5.3, 6.1, 7.9, 8.1, 9.8};
  unsigned int uint_data[count] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char *str_data[count] = {"apple", "banana", "carrot", "durian", "eel", "frankfurter", "gelato", "harissa", "icecream", "jam"};
  char *str_data2[str_count] = {"4PGC938", "2IYE230", "3CIO720", "1ICK750", "1OHV845", "4JZY524", "1ICK750", "3CIO720", "1OHV845", "1OHV845", "2RLA629", "2RLA629", "3ATW723"};
  char *str_data3[str_count] = {"she", "sells", "seashells", "by", "the", "seashore", "the", "shells", "she", "sells", "are", "surely", "seashells"};
  char char_data[count] = "abcdefghij";
  custom_t custom_data[count] = {{.name = "Alice", .id = 0}, {.name = "Bob", .id = 1}, 
                          {.name = "Charlie", .id = 2}, {.name = "David", .id = 3},
                          {.name = "Eve", .id = 4}, {.name = "Frank", .id = 5},
                          {.name = "Greg", .id = 6}, {.name = "Harriet", .id = 7},
                          {.name = "Ingrid", .id = 8}, {.name = "Janet", .id = 9}};
  int permutation_data[count] = {8, 0, 2, 4, 6, 7, 9, 1, 3, 5};  // arbitrary permutation for shuffling

  for (int i = 0; i < count; i++) {
    int_arr[i] = int_data[i];
    int_dup_arr[i] = int_dup_data[i];
    float_arr[i] = float_data[i];
    double_arr[i] = double_data[i];
    uint_arr[i] = uint_data[i];
    str_arr[i] = str_data[i];
    char_arr[i] = char_data[i];
    custom_arr[i] = custom_data[i];
    permutation[i] = permutation_data[i];
  }

  for (int i = 0; i < str_count; i++) {
    str_arr2[i] = str_data2[i];
    str_arr3[i] = str_data3[i];
  }
}

void tearDown() {
}

// Helper functions/structures
bool less_custom_name(void *v, void *w) {
  char *vname = (*(custom_t *)v).name;
  char *wname = (*(custom_t *)w).name;
  return (strcmp(vname, wname) < 0);
}

bool less_custom_id(void *v, void *w) {
  int vid = (*(custom_t *)v).id;
  int wid = (*(custom_t *)w).id;
  return vid < wid;
}

// Shuffles an array using the given permutation
void shuffle_with_permutation(void *arr, size_t item_size, size_t n, int *permutation) {
  uint8_t new_arr[item_size * n];

  for (int i = 0; i < n; i++) {
    int j = permutation[i];
    memcpy(new_arr + item_size * i, arr + item_size * j, item_size);
  }
  memcpy(arr, new_arr, item_size * n);
}

// Tests

// Tests that the sort library's shuffle function works 
void test_shuffle() {
  int len = 1000;
  int shuffle_arr[len];   // array to be shuffled
  int shuffle_count[len]; // array to count occurrences in shuffled array

  // Fill the arrays
  for (int i = 0; i < len; i++) {
    shuffle_arr[i] = i;
    shuffle_count[i] = 0;
  }
  
  // Randomly shuffle the array
  shuffle(shuffle_arr, sizeof(int), len);

  // Tally occurrences of each value
  for (int i = 0; i < len; i++) {
    int value = shuffle_arr[i];
    TEST_ASSERT_TRUE(value >= 0);
    TEST_ASSERT_TRUE(value < len);

    shuffle_count[value] += 1;
  }

  // Check that each value occurred once
  for (int i = 0; i < len; i++) {
    TEST_ASSERT_TRUE(shuffle_count[i] == 1);
  }
}

// Tests that pre-sorted arrays correctly identify as such
void test_int_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int));
  int_arr[0] = int_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int));
}

void test_int_dup_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(int_dup_arr, sizeof(int_dup_arr[0]), NELEMS(int_dup_arr), less_int));
}

void test_float_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float));
  float_arr[0] = float_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float));
}

void test_double_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double));
  double_arr[0] = double_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double)); 
}

void test_uint_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint));
  uint_arr[0] = uint_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint));  
}

void test_str_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str));
  str_arr[0] = str_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str));  
}

void test_char_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char));
  char_arr[0] = char_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char));  
}

void test_custom_is_sorted() {
  TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name));
  TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id));
  custom_arr[0] = custom_arr[count - 1];
  TEST_ASSERT_FALSE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name));
  TEST_ASSERT_FALSE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id));
}

// Actual sorting tests
void test_int_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), permutation);  
    sort(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int, types[i]);
    TEST_ASSERT_TRUE(is_sorted(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int));
  }
}

void test_float_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), permutation);
    sort(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float, types[i]);
    TEST_ASSERT_TRUE(is_sorted(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float));
  }
}

void test_double_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), permutation);
    sort(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double, types[i]);
    TEST_ASSERT_TRUE(is_sorted(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double));
  }
}

void test_str_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), permutation);
    sort(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str, types[i]);
    TEST_ASSERT_TRUE(is_sorted(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str));
  }
}

void test_char_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), permutation);
    sort(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char, types[i]);
    TEST_ASSERT_TRUE(is_sorted(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char));
  }
}

void test_uint_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), permutation);
    sort(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint, types[i]);
    TEST_ASSERT_TRUE(is_sorted(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint));
  }
}

void test_custom_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    shuffle_with_permutation(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), permutation);
    sort(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id, types[i]);
    TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id));

    shuffle_with_permutation(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), permutation);
    sort(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name, types[i]);
    TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name));
  }
}

void test_int_random_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    int numbers = 1000;
    int int_rand_arr[numbers];
    sort(int_rand_arr, sizeof(int_rand_arr[0]), NELEMS(int_rand_arr), less_int, types[i]);
    TEST_ASSERT_TRUE(is_sorted(int_rand_arr, sizeof(int_rand_arr[0]), NELEMS(int_rand_arr), less_int));
  }
}

void test_text_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    char *text = strdup("SORTEXAMPLE");
    sort(text, sizeof(char), strlen(text), less_char, types[i]);
    TEST_ASSERT_TRUE(is_sorted(text, sizeof(char), strlen(text), less_char));
    free(text);
  }
}

void test_empty_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    int empty[0];
    sort(empty, sizeof(int), 0, less_int, types[i]);
    TEST_ASSERT_TRUE(is_sorted(empty, sizeof(int), 0, less_int));
  }
}

void test_one_sort() {
  for (int i = 0; i < NELEMS(types); i++) {
    setUp();
    int one[1] = {1};
    sort(one, sizeof(int), 1, less_int, types[i]);
    TEST_ASSERT_TRUE(is_sorted(one, sizeof(int), 1, less_int));
  }
}

void test_dedicated_str_sort() {
  for (int i = 0; i < NELEMS(str_sort_types); i++) {
    // printf("String sort #%d\n", i);
    setUp();
    
    sort(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str, types[i]);
    TEST_ASSERT_TRUE(is_sorted(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str));

    sort(str_arr2, sizeof(str_arr2[0]), NELEMS(str_arr2), less_str, str_sort_types[i]);
    TEST_ASSERT_TRUE(is_sorted(str_arr2, sizeof(str_arr2[0]), NELEMS(str_arr3), less_str));
    
    sort(str_arr3, sizeof(str_arr3[0]), NELEMS(str_arr3), less_str, str_sort_types[i]);
    TEST_ASSERT_TRUE(is_sorted(str_arr3, sizeof(str_arr3[0]), NELEMS(str_arr3), less_str));
  }
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_shuffle);

  RUN_TEST(test_int_is_sorted);
  RUN_TEST(test_uint_is_sorted);
  RUN_TEST(test_int_dup_is_sorted);
  RUN_TEST(test_float_is_sorted);
  RUN_TEST(test_double_is_sorted);
  RUN_TEST(test_str_is_sorted);
  RUN_TEST(test_char_is_sorted);
  RUN_TEST(test_custom_is_sorted);
  
  RUN_TEST(test_int_sort);
  RUN_TEST(test_uint_sort);
  RUN_TEST(test_float_sort);
  RUN_TEST(test_double_sort);
  RUN_TEST(test_str_sort);
  RUN_TEST(test_char_sort);
  RUN_TEST(test_custom_sort);

  RUN_TEST(test_int_random_sort);
  RUN_TEST(test_text_sort);
  RUN_TEST(test_empty_sort);
  RUN_TEST(test_one_sort);

  RUN_TEST(test_dedicated_str_sort);

  return UNITY_END();
}
