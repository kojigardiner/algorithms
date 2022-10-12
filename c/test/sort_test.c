#include "../../../Unity/src/unity.h"
#include "../sort/sort.h"
#include "string.h"
#include "stdlib.h"

#define RAND_SEED 0

// Initially sorted arrays as input
typedef struct custom {
  char name[16];
  int id;
} custom_t;

static const int count = 5;
static int int_arr[count];
static int int_dup_arr[count];
static float float_arr[count];
static double double_arr[count];
static unsigned int uint_arr[count];
static char *str_arr[count];
static char char_arr[count];
static int permutation[count];
custom_t custom_arr[count];

void setUp() {
  srand(RAND_SEED);   // seed PRNG

  // Reset global data
  int int_data[count] = {-1, 0, 1, 2, 3};
  int int_dup_data[count] = {1, 1, 1, 1, 1};
  float float_data[count] = {0.1, 1.2, 2.4, 3.3, 4.7};
  double double_data[count] = {0.1, 1.2, 2.4, 3.3, 4.7};
  unsigned int uint_data[count] = {0, 1, 2, 3, 4};
  char *str_data[count] = {"apple", "banana", "carrot", "durian", "eel"};
  char char_data[count] = "abcde";
  custom_t custom_data[count] = {{.name = "Alice", .id = 0}, {.name = "Bob", .id = 1}, 
                          {.name = "Charlie", .id = 2}, {.name = "David", .id = 3},
                          {.name = "Eve", .id = 4}};
  int permutation_data[count] = {3, 0, 2, 4, 1};  // arbitrary permutation for shuffling

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
void shuffle(void *arr, size_t item_size, size_t n, int *permutation) {
  uint8_t new_arr[item_size * n];

  for (int i = 0; i < n; i++) {
    int j = permutation[i];
    memcpy(new_arr + item_size * i, arr + item_size * j, item_size);
  }
  memcpy(arr, new_arr, item_size * n);
}

// Tests

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

void test_int_sort() {
  shuffle(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), permutation);
  sort(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int);
  TEST_ASSERT_TRUE(is_sorted(int_arr, sizeof(int_arr[0]), NELEMS(int_arr), less_int));
}

void test_float_sort() {
  shuffle(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), permutation);
  sort(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float);
  TEST_ASSERT_TRUE(is_sorted(float_arr, sizeof(float_arr[0]), NELEMS(float_arr), less_float));
}

void test_double_sort() {
  shuffle(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), permutation);
  sort(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double);
  TEST_ASSERT_TRUE(is_sorted(double_arr, sizeof(double_arr[0]), NELEMS(double_arr), less_double));
}

void test_str_sort() {
  shuffle(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), permutation);
  sort(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str);
  TEST_ASSERT_TRUE(is_sorted(str_arr, sizeof(str_arr[0]), NELEMS(str_arr), less_str));
}

void test_char_sort() {
  shuffle(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), permutation);
  sort(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char);
  TEST_ASSERT_TRUE(is_sorted(char_arr, sizeof(char_arr[0]), NELEMS(char_arr), less_char));
}

void test_uint_sort() {
  shuffle(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), permutation);
  sort(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint);
  TEST_ASSERT_TRUE(is_sorted(uint_arr, sizeof(uint_arr[0]), NELEMS(uint_arr), less_uint));
}

void test_custom_sort() {
  shuffle(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), permutation);
  sort(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id);
  TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_id));
  
  shuffle(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), permutation);
  sort(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name);
  TEST_ASSERT_TRUE(is_sorted(custom_arr, sizeof(custom_arr[0]), NELEMS(custom_arr), less_custom_name));
}

void test_int_random_sort() {
  int numbers = 1000;
  int int_rand_arr[numbers];
  for (int i = 0; i < numbers; i++) {
    int_rand_arr[i] = rand();
  }

  sort(int_rand_arr, sizeof(int_rand_arr[0]), NELEMS(int_rand_arr), less_int);
  TEST_ASSERT_TRUE(is_sorted(int_rand_arr, sizeof(int_rand_arr[0]), NELEMS(int_rand_arr), less_int));
}

// Main
int main() {
  UNITY_BEGIN();
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

  return UNITY_END();
}
