#include "../../../Unity/src/unity.h"
#include "../graph/graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Text file format:
// num vertices
// num edges
// v w
static const int num_v = 13;
static const int num_e = 22;
static const int connections[num_e][2] = {{4, 2}, {2, 3}, {3, 2}, {6, 0}, \
                                          {0, 1}, {2, 0}, {11, 12}, {12, 9},\
                                          {9, 10}, {9, 11}, {7, 9}, {10, 12},\
                                          {11, 4}, {4, 3}, {3, 5}, {6, 8},\
                                          {8, 6}, {5, 4}, {0, 5}, {6, 4},\
                                          {6, 9}, {7, 6}};

graph_t *g;

void setUp() {
  g = graph_init(num_v, DIRECTED);
}

void tearDown() {
  if (g) {
    graph_free(g);
  }
}

// Helper function, fills graph
void fill_graph() {
  for (int i = 0; i < num_e; i++) {
    TEST_ASSERT_TRUE(graph_add_edge(g, connections[i][0], connections[i][1]));
  }
}

// Helper function, fills graph in reverse
void fill_graph_reverse() {
  for (int i = 0; i < num_e; i++) {
    TEST_ASSERT_TRUE(graph_add_edge(g, connections[i][1], connections[i][0]));
  }
}

// Tests
// Tests creation of a graph with 0 vertices.
void test_empty_graph_fails() {
  TEST_ASSERT_NULL(graph_init(0, DIRECTED));
}

// Tests number of vertices is correct.
void test_vertex_count() {
  TEST_ASSERT_EQUAL(num_v, graph_V(g));
}

// Tests number of edges is correct.
void test_edge_count() {
  TEST_ASSERT_EQUAL(0, graph_E(g));
  fill_graph();
  TEST_ASSERT_EQUAL(num_e, graph_E(g));
}

// Test adjacent iterator failure
void test_adj_iter_empty() {
  TEST_ASSERT_FALSE(graph_adj_iter_init(g, 0));
  TEST_ASSERT_FALSE(graph_adj_iter_init(g, 22));
}

// Test adjacent iterator. Note that the order of values assumes the underlying 
// implementation uses a linked adjacency list for each vertex.
void test_adj_iter() {
  fill_graph();

  int v = 0;
  int w;
  int expected[] = {5, 1};
  graph_adj_iter_init(g, v);
  int i = 0;
  while (graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(graph_adj_iter_next(g, v, &w));
    TEST_ASSERT_EQUAL(expected[i], w);
    i++;
  }

  v = 7;
  int expected2[] = {6, 9};
  graph_adj_iter_init(g, v);
  i = 0;
  while (graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(graph_adj_iter_next(g, v, &w));
    TEST_ASSERT_EQUAL(expected2[i], w);
    i++;
  }
}

// Test adjacent iterator with repeated call on same vertex.
void test_adj_iter_repeat() {
  fill_graph();

  int v = 0;
  int w;
  int expected[] = {5, 1};
  graph_adj_iter_init(g, v);
  int i = 0;
  while (graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(graph_adj_iter_next(g, v, &w));
    TEST_ASSERT_EQUAL(expected[i], w);
    i++;
  }

  graph_adj_iter_init(g, v);
  i = 0;
  while (graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(graph_adj_iter_next(g, v, &w));
    TEST_ASSERT_EQUAL(expected[i], w);
    i++;
  }
}

// Tests the adjacent iterator on an unconnected vertex
void test_adj_iter_unconnected() {
  int v = 0;
  TEST_ASSERT_FALSE(graph_adj_iter_init(g, v));
}

// Tests the adjacent iterator with out of bounds values.
void test_adj_iter_out_of_bounds() {
  fill_graph();
  TEST_ASSERT_FALSE(graph_adj_iter_init(g, -1));
  TEST_ASSERT_FALSE(graph_adj_iter_init(g, num_v));
  graph_adj_iter_init(g, 0);
  TEST_ASSERT_FALSE(graph_adj_iter_has_next(g, -1));
  TEST_ASSERT_FALSE(graph_adj_iter_has_next(g, num_v));
  
  int w;
  TEST_ASSERT_FALSE(graph_adj_iter_next(g, -1, &w));
  TEST_ASSERT_FALSE(graph_adj_iter_next(g, num_v, &w));
}

// Tests adding edges to out-of-bounds vertices.
void test_add_edge_out_of_bounds() {
  fill_graph();
  TEST_ASSERT_FALSE(graph_add_edge(g, -1, 1));
  TEST_ASSERT_FALSE(graph_add_edge(g, num_v, 1));
  TEST_ASSERT_FALSE(graph_add_edge(g, 1, num_v));
  TEST_ASSERT_FALSE(graph_add_edge(g, 1, -1));
  TEST_ASSERT_FALSE(graph_add_edge(g, -1, -1));
  TEST_ASSERT_FALSE(graph_add_edge(g, num_v, num_v));
}

// Test print
void test_print() {
  fill_graph();
  graph_print(g);
}

// Test that a reversed graph has the same connections as a graph initialized
// with reverse edges.
void test_reverse() {
  fill_graph();
  graph_t *g_r = graph_reverse(g);

  TEST_ASSERT_EQUAL(graph_V(g), graph_V(g_r));
  TEST_ASSERT_EQUAL(graph_E(g), graph_E(g_r));

  int expected[] = {11, 6, 5};
  int v = 4;
  int w_r;
  int i = 0;
  graph_adj_iter_init(g_r, v);
  while(graph_adj_iter_has_next(g_r, v)) {
    graph_adj_iter_next(g_r, v, &w_r);
    TEST_ASSERT_EQUAL(expected[i], w_r);
    i++;
  }
}

// Main
int main() {
  UNITY_BEGIN();
  
  RUN_TEST(test_empty_graph_fails);
  RUN_TEST(test_vertex_count);
  RUN_TEST(test_edge_count);
  RUN_TEST(test_adj_iter_empty);
  RUN_TEST(test_adj_iter);
  RUN_TEST(test_adj_iter_repeat);
  RUN_TEST(test_adj_iter_unconnected);
  RUN_TEST(test_adj_iter_out_of_bounds);
  RUN_TEST(test_add_edge_out_of_bounds);
  RUN_TEST(test_print);
  RUN_TEST(test_reverse);

  return UNITY_END();
}
