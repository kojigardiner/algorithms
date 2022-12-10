#include "../../../Unity/src/unity.h"
#include "../edge/edge.h"
#include <string.h>

void setUp() {
}

void tearDown() {
}
// Tests

// Tests that a new bag is empty
void test_edge_init() {
  edge_t *e = edge_init(2, 3, 0.5);
  TEST_ASSERT_NOT_NULL(e);

  double weight = edge_weight(e);
  TEST_ASSERT_EQUAL(0.5, weight);

  int v = edge_either(e);
  TEST_ASSERT_EQUAL(2, v);

  int w = edge_other(e, v);
  TEST_ASSERT_EQUAL(3, w);

  v = edge_other(e, w);
  TEST_ASSERT_EQUAL(2, v);

  v = edge_from(e);
  TEST_ASSERT_EQUAL(2, v);

  w = edge_to(e);
  TEST_ASSERT_EQUAL(3, w);
}

// Test comparison
void test_edge_compare_unequal() {
  edge_t *e1 = edge_init(2, 3, 0.5);
  edge_t *e2 = edge_init(4, 99, 0.1);

  TEST_ASSERT_EQUAL(1, edge_compare(e1, e2));
  TEST_ASSERT_EQUAL(-1, edge_compare(e2, e1));
}

void test_edge_compare_equal() {
  edge_t *e1 = edge_init(2, 3, 0.1);
  edge_t *e2 = edge_init(4, 99, 0.1);

  TEST_ASSERT_EQUAL(0, edge_compare(e1, e2));
}

void test_edge_compare_negative() {
  edge_t *e1 = edge_init(2, 3, -0.5);
  edge_t *e2 = edge_init(4, 99, 0.1);

  TEST_ASSERT_EQUAL(-1, edge_compare(e1, e2));
  TEST_ASSERT_EQUAL(1, edge_compare(e2, e1));
}

void test_edge_less() {
  edge_t *e1 = edge_init(2, 3, -0.5);
  edge_t *e2 = edge_init(4, 99, 0.1);

  TEST_ASSERT_TRUE(edge_less(&e1, &e2));
  TEST_ASSERT_FALSE(edge_less(&e2, &e1));
  TEST_ASSERT_FALSE(edge_less(&e1, &e1));
}

void test_edge_print() {
  edge_t *e1 = edge_init(2, 3, -0.5);
  edge_t *e2 = edge_init(4, 99, 0.1);  

  edge_print(e1);
  edge_print(e2);
}

// Test edge_free
void test_edge_free() {
  edge_t *e = edge_init(4, 99, 0.1);

  edge_free(e);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_edge_init);
  RUN_TEST(test_edge_compare_unequal);
  RUN_TEST(test_edge_compare_equal);
  RUN_TEST(test_edge_compare_negative);
  RUN_TEST(test_edge_less);
  RUN_TEST(test_edge_print);
  RUN_TEST(test_edge_free);
  return UNITY_END();
}
