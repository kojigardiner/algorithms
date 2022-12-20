#include "../../../Unity/src/unity.h"
#include "../shortest_paths/shortest_paths.h"
#include "../edge/edge.h"
#include "../weighted_graph/weighted_graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void setUp() {
}

void tearDown() {
}

// Tests

void test_sp() {
  weighted_graph_t *g = weighted_graph_init_file("../data/tinyEWD.txt", DIRECTED);

  int s = 0;
  sp_t *sp = sp_init(g, s);   // all paths from vertex 0
  TEST_ASSERT_NOT_NULL(sp);

  double expected_dist[] = {0.00, 1.05, 0.26, 0.99, 0.38, 0.73, 1.51, 0.60};

  // Check all paths
  for (int i = 0; i < weighted_graph_V(g); i++) {
    TEST_ASSERT_TRUE(sp_has_path_to(sp, i));

    double dist = sp_dist_to(sp, i);
    TEST_ASSERT_EQUAL_DOUBLE(expected_dist[i], dist);

    edge_t *e;
    int v, w;
    double weight;

    printf("%d to %d (%.2lf): ", s, i, dist);
    TEST_ASSERT_TRUE(sp_iter_init(sp, i));
    while (sp_iter_has_next(sp, i)) {
      sp_iter_next(sp, i, &e);
      v = edge_from(e);
      w = edge_to(e);
      weight = edge_weight(e);

      printf("%d->%d %.2lf ", v, w, weight);
    }
    printf("\n");
  }
  
  sp_free(sp);
}

// Tests with graphs that are not allowed
void test_sp_undirected() {
  weighted_graph_t *g = weighted_graph_init(5, UNDIRECTED);
  edge_t *e = edge_init(0, 1, 1.2);
  weighted_graph_add_edge(g, e);

  sp_t *sp = sp_init(g, 0);
  TEST_ASSERT_NULL(sp);
}

void test_sp_negative_weight() {
  weighted_graph_t *g = weighted_graph_init(5, DIRECTED);
  edge_t *e = edge_init(0, 1, -1.2);
  weighted_graph_add_edge(g, e);

  sp_t *sp = sp_init(g, 0);
  TEST_ASSERT_NULL(sp);
}

// Main
int main() {
  UNITY_BEGIN();

  RUN_TEST(test_sp);
  RUN_TEST(test_sp_undirected);
  RUN_TEST(test_sp_negative_weight);

  return UNITY_END();
}
