#include "../../../Unity/src/unity.h"
#include "../cycle/cycle.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void setUp() {
}

void tearDown() {
}

// Tests

void test_undirected_cycle_true() {
  graph_t *g = graph_init(4, UNDIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 0, 3);

  cycle_t *c = cycle_init(g);
  TEST_ASSERT_TRUE(has_cycle(c));
}

void test_undirected_cycle_false() {
  graph_t *g = graph_init(4, UNDIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 2, 3);

  cycle_t *c = cycle_init(g);
  TEST_ASSERT_FALSE(has_cycle(c));
}

void test_directed_cycle_true() {
  graph_t *g = graph_init(4, DIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 3, 0);

  cycle_t *c = cycle_init(g);
  TEST_ASSERT_TRUE(has_cycle(c));
}

void test_directed_cycle_false() {
  graph_t *g = graph_init(4, DIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 0, 3);

  cycle_t *c = cycle_init(g);
  TEST_ASSERT_FALSE(has_cycle(c));
}

void test_directed_cycle_iter() {
  graph_t *g = graph_init(13, DIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 0, 5);
  graph_add_edge(g, 2, 0);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 3, 2);
  graph_add_edge(g, 3, 5);
  graph_add_edge(g, 4, 3);
  graph_add_edge(g, 4, 2);
  graph_add_edge(g, 5, 4);
  graph_add_edge(g, 6, 0);
  graph_add_edge(g, 6, 4);
  graph_add_edge(g, 6, 8);
  graph_add_edge(g, 6, 9);
  graph_add_edge(g, 7, 6);
  graph_add_edge(g, 7, 9);
  graph_add_edge(g, 8, 6);
  graph_add_edge(g, 9, 10);
  graph_add_edge(g, 9, 11);
  graph_add_edge(g, 10, 12);
  graph_add_edge(g, 11, 4);
  graph_add_edge(g, 11, 12);
  graph_add_edge(g, 12, 9);

  cycle_t *c = cycle_init(g);
  TEST_ASSERT_TRUE(has_cycle(c));

  TEST_ASSERT_TRUE(cycle_iter_init(c));
  
  int expected[] = {3, 5, 4, 2, 3};
  int i = 0;
  while (cycle_iter_has_next(c)) {
    int v;
    cycle_iter_next(c, &v);
    TEST_ASSERT_EQUAL(expected[i], v);
    printf(" -> %d", v);
    i++;
  }
  printf("\n");
}

// Main
int main() {
  UNITY_BEGIN();
  
  RUN_TEST(test_undirected_cycle_true);
  RUN_TEST(test_undirected_cycle_false);
  RUN_TEST(test_directed_cycle_true);
  RUN_TEST(test_directed_cycle_false);
  RUN_TEST(test_directed_cycle_iter);

  return UNITY_END();
}