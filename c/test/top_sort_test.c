#include "../../../Unity/src/unity.h"
#include "../topological_sort/topological_sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void setUp() {
}

void tearDown() {
}

// Tests
void test_top_sort_iter() {
  graph_t *g = graph_init(13, DIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 0, 5);
  graph_add_edge(g, 0, 6);
  graph_add_edge(g, 2, 0);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 3, 5);
  graph_add_edge(g, 5, 4);
  graph_add_edge(g, 6, 4);
  graph_add_edge(g, 6, 9);
  graph_add_edge(g, 7, 6);
  graph_add_edge(g, 8, 7);
  graph_add_edge(g, 9, 10);
  graph_add_edge(g, 9, 11);
  graph_add_edge(g, 9, 12);
  graph_add_edge(g, 11, 12);

  top_sort_t *ts = top_sort_init(g);
  TEST_ASSERT_NOT_NULL(ts);

  TEST_ASSERT_TRUE(top_sort_iter_init(ts));
  
  int expected[] = {8, 7, 2, 3, 0, 1, 5, 6, 4, 9, 10, 11, 12};
  int i = 0;
  while (top_sort_iter_has_next(ts)) {
    int v;
    top_sort_iter_next(ts, &v);
    TEST_ASSERT_EQUAL(expected[i], v);
    printf(" -> %d", v);
    i++;
  }
  printf("\n");

  top_sort_free(ts);
}

// Main
int main() {
  UNITY_BEGIN();
  
  RUN_TEST(test_top_sort_iter);

  return UNITY_END();
}