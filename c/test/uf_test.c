#include "../../../Unity/src/unity.h"
#include "../union_find/uf.h"

static int p_values[10] = {4, 3, 6, 9, 2, 8, 5, 7, 6};
static int q_values[10] = {3, 8, 5, 4, 1, 9, 0, 2, 1};

static uf_t *uf;

void setUp() {
}

void tearDown() {
}

// Tests

// Tests that a new uf can be instantiated with multiple sizes
void test_uf_init() {
  uf = uf_init(5);
  TEST_ASSERT_EQUAL(5, uf_count(uf));

  uf = uf_init(1000);
  TEST_ASSERT_EQUAL(1000, uf_count(uf));

  uf = uf_init(-1);
  TEST_ASSERT_NULL(uf);
}

void test_uf_simple() {
  uf = uf_init(2);
  uf_union(uf, 0, 0);
  TEST_ASSERT_EQUAL(2, uf_count(uf));
  TEST_ASSERT_FALSE(uf_connected(uf, 0, 1));

  uf_union(uf, 0, 1);
  TEST_ASSERT_EQUAL(1, uf_count(uf));
  TEST_ASSERT_TRUE(uf_connected(uf, 0, 1));
}

void test_uf_larger() {
  uf = uf_init(10);
  for (int i = 0; i < 10; i++) {
    uf_union(uf, p_values[i], q_values[i]);
    TEST_ASSERT_TRUE(uf_connected(uf, p_values[i], q_values[i]));
  }
  TEST_ASSERT_EQUAL(2, uf_count(uf));
  TEST_ASSERT_TRUE(uf_connected(uf, 0, 7));
  TEST_ASSERT_TRUE(uf_connected(uf, 8, 9));
  TEST_ASSERT_FALSE(uf_connected(uf, 7, 8));
}

void test_uf_out_of_bounds() {
  uf = uf_init(2);
  uf_union(uf, 0, 2);
  uf_union(uf, 2, 0);
  uf_union(uf, 2, 2);
  uf_union(uf, -1, 1);
  uf_union(uf, -1, -1);
  uf_union(uf, 0, -1);
  TEST_ASSERT_EQUAL(uf_count(uf), 2);
}

void test_uf_free() {
  uf = uf_init(10);
  uf_free(uf);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_uf_init);
  RUN_TEST(test_uf_simple);
  RUN_TEST(test_uf_larger);
  RUN_TEST(test_uf_out_of_bounds);
  RUN_TEST(test_uf_free);
  return UNITY_END();
}
