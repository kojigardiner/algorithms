#include "../../../Unity/src/unity.h"
#include "../weighted_graph/weighted_graph.h"
#include "../edge/edge.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Text file format:
// num vertices
// num edges
// v w
static bool init_with_file = false;
static char *filename = "../data/tinyEWG.txt";
static const int num_v = 8;
static const int num_e = 16;
static const int connections[num_e][2] = {{4, 5}, {4, 7}, {5, 7}, {0, 7}, \
                                              {1, 5}, {0, 4}, {2, 3}, {1, 7},\
                                              {0, 2}, {1, 2}, {1, 3}, {2, 7},\
                                              {6, 2}, {3, 6}, {6, 0}, {6, 4}};
static const double weights[num_e] = {0.35, 0.37, 0.28, 0.16, 0.32, 0.38, 0.17,\
                                      0.19, 0.26, 0.36, 0.29, 34, 0.40, 0.52,\
                                      0.58, 0.93};

weighted_graph_t *g;

void setUp() {
  if (!init_with_file) {
    g = weighted_graph_init(num_v, UNDIRECTED);
  } else {
    g = weighted_graph_init_file(filename, UNDIRECTED);
  }
}

void tearDown() {
  if (g) {
    weighted_graph_free(g);
  }
}

// Helper function, fills graph
void fill_graph() {
  if (!init_with_file) {
    for (int i = 0; i < num_e; i++) {
      edge_t *e = edge_init(connections[i][0], connections[i][1], weights[i]);
      TEST_ASSERT_TRUE(weighted_graph_add_edge(g, e));
    }
  }
}

// Tests
// Tests creation of a graph with 0 vertices.
void test_empty_graph_fails() {
  TEST_ASSERT_NULL(weighted_graph_init(0, UNDIRECTED));
}

// Tests number of vertices is correct.
void test_vertex_count() {
  TEST_ASSERT_EQUAL(num_v, weighted_graph_V(g));
}

// Tests number of edges is correct.
void test_edge_count() {
  if (!init_with_file) {
    TEST_ASSERT_EQUAL(0, weighted_graph_E(g));
    fill_graph();
  }
  TEST_ASSERT_EQUAL(num_e, weighted_graph_E(g));
}

// Test adjacent iterator failure
void test_adj_iter_empty() {
  if (!init_with_file) {
    TEST_ASSERT_FALSE(weighted_graph_adj_iter_init(g, 0));
    TEST_ASSERT_FALSE(weighted_graph_adj_iter_init(g, 13));
  }
}

// Test adjacent iterator. Note that the order of values assumes the underlying 
// implementation uses a linked adjacency list for each vertex.
void test_adj_iter() {
  fill_graph();

  int v = 0;
  int expectedV[] = {6, 0, 0, 0};
  int expectedW[] = {0, 2, 4, 7};
  double expectedWeight[] = {0.58, 0.26, 0.38, 0.16};
  weighted_graph_adj_iter_init(g, v);
  int i = 0;
  int v_check, w_check;
  double weight_check;
  edge_t *e;
  while (weighted_graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(weighted_graph_adj_iter_next(g, v, &e));
    v_check = edge_either(e);
    w_check = edge_other(e, v_check);
    weight_check = edge_weight(e);

    TEST_ASSERT_EQUAL(expectedV[i], v_check);
    TEST_ASSERT_EQUAL(expectedW[i], w_check);
    TEST_ASSERT_EQUAL(expectedWeight[i], weight_check);
    i++;
  }

  v = 5;
  int expectedV2[] = {1, 5, 4};
  int expectedW2[] = {5, 7, 5};
  double expectedWeight2[] = {0.32, 0.28, 0.35};
  weighted_graph_adj_iter_init(g, v);
  i = 0;
  while (weighted_graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(weighted_graph_adj_iter_next(g, v, &e));
    v_check = edge_either(e);
    w_check = edge_other(e, v_check);
    weight_check = edge_weight(e);

    TEST_ASSERT_EQUAL(expectedV2[i], v_check);
    TEST_ASSERT_EQUAL(expectedW2[i], w_check);
    TEST_ASSERT_EQUAL(expectedWeight2[i], weight_check);
    i++;
  }
}

// Test adjacent iterator with repeated call on same vertex.
void test_adj_iter_repeat() {
  fill_graph();

  int v = 0;
  int expectedV[] = {6, 0, 0, 0};
  int expectedW[] = {0, 2, 4, 7};
  double expectedWeight[] = {0.58, 0.26, 0.38, 0.16};
  weighted_graph_adj_iter_init(g, v);
  int i = 0;
  int v_check, w_check;
  double weight_check;
  edge_t *e;
  while (weighted_graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(weighted_graph_adj_iter_next(g, v, &e));
    v_check = edge_either(e);
    w_check = edge_other(e, v_check);
    weight_check = edge_weight(e);

    TEST_ASSERT_EQUAL(expectedV[i], v_check);
    TEST_ASSERT_EQUAL(expectedW[i], w_check);
    TEST_ASSERT_EQUAL(expectedWeight[i], weight_check);
    i++;
  }

  weighted_graph_adj_iter_init(g, v);
  i = 0;
  while (weighted_graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(weighted_graph_adj_iter_next(g, v, &e));
    v_check = edge_either(e);
    w_check = edge_other(e, v_check);
    weight_check = edge_weight(e);

    TEST_ASSERT_EQUAL(expectedV[i], v_check);
    TEST_ASSERT_EQUAL(expectedW[i], w_check);
    TEST_ASSERT_EQUAL(expectedWeight[i], weight_check);
    i++;
  }
}

// Tests the adjacent iterator with out of bounds values.
void test_adj_iter_out_of_bounds() {
  fill_graph();
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_init(g, -1));
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_init(g, num_v));
  weighted_graph_adj_iter_init(g, 0);
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_has_next(g, -1));
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_has_next(g, num_v));
  
  edge_t *e;
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_next(g, -1, &e));
  TEST_ASSERT_FALSE(weighted_graph_adj_iter_next(g, num_v, &e));
}

// Tests adding edges to out-of-bounds vertices.
void test_add_edge_out_of_bounds() {
  fill_graph();
  edge_t *e;
  e = edge_init(-1, 1, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
  e = edge_init(num_v, 1, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
  e = edge_init(1, num_v, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
  e = edge_init(1, -1, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
  e = edge_init(-1, -1, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
  e = edge_init(num_v, num_v, 0.5);
  TEST_ASSERT_FALSE(weighted_graph_add_edge(g, e));
}

// Test the all edges iterator
void test_edge_iter() {
  fill_graph();

  TEST_ASSERT_TRUE(weighted_graph_edge_iter_init(g));

  edge_t *e;
  int v_check, w_check;
  double weight_check;

  int expectedV[] = {6, 0, 0, 0, 1, 1, 1, 1, 6, 2, 1, 0, 2, 3, 1, 2, 6, 0, 4, 4, 1, 5, 4, 6, 6, 3, 6, 2, 1, 0, 5, 4};
  int expectedW[] = {0, 2, 4, 7, 3, 2, 7, 5, 2, 7, 2, 2, 3, 6, 3, 3, 4, 4, 7, 5, 5, 7, 5, 4, 0, 6, 2, 7, 7, 7, 7, 7};
  double expectedWeight[] = {.58, .26, .38, .16, .29, .36, .19, .32, .40, .34, .36, .26, .17, .52, .29, .17, .93, .38, .37, .35, .32, .28, .35, .93, .58, .52, .40, .34, .19, .16, .28, .37};
  int i = 0;
  while (weighted_graph_edge_iter_has_next(g)) {
    weighted_graph_edge_iter_next(g, &e);
    v_check = edge_either(e);
    w_check = edge_other(e, v_check);
    weight_check = edge_weight(e);

    TEST_ASSERT_EQUAL(expectedV[i], v_check);
    TEST_ASSERT_EQUAL(expectedW[i], w_check);
    TEST_ASSERT_EQUAL(expectedWeight[i], weight_check);
    i++;
  }
}

// Test print
void test_print() {
  fill_graph();
  weighted_graph_print(g);
}

// Test that trying to reverse an undirected graph fails.
// void test_reverse_undirected() {
//   TEST_ASSERT_NULL(weighted_graph_reverse(g));
// }

// Main
int main() {
  UNITY_BEGIN();

  // Run test once with regular init, and second time with init with file  
  for (int i = 0; i < 2; i++) {
    if (i == 1) {
      init_with_file = true;
    }
    RUN_TEST(test_empty_graph_fails);
    RUN_TEST(test_vertex_count);
    RUN_TEST(test_edge_count);
    RUN_TEST(test_adj_iter_empty);
    RUN_TEST(test_adj_iter);
    RUN_TEST(test_adj_iter_repeat);
    RUN_TEST(test_adj_iter_out_of_bounds);
    RUN_TEST(test_add_edge_out_of_bounds);
    RUN_TEST(test_print);
    // RUN_TEST(test_reverse_undirected);
  }

  return UNITY_END();
}
