#include "../../../Unity/src/unity.h"
#include "../graph/graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Text file format:
// num vertices
// num edges
// v w
static bool init_with_file = false;
static char *filename = "../data/tinyG.txt";
static const int num_v = 13;
static const int num_e = 13;
static const int connections[num_e][2] = {{0, 5}, {4, 3}, {0, 1}, {9, 12}, \
                                              {6, 4}, {5, 4}, {0, 2}, {11, 12},\
                                              {9, 10}, {0, 6}, {7, 8}, {9, 11},\
                                              {5, 3}};
// static char *input = "13\n13\n0 5\n4 3\n0 1\n9 12\n6 4\n5 4\n0 2\n11 12\n9 10\n0 6\n7 8\n9 11\n5 3\n";

graph_t *g;

void setUp() {
  if (!init_with_file) {
    g = graph_init(num_v, UNDIRECTED);
  } else {
    g = graph_init_file(filename, UNDIRECTED);
  }
}

void tearDown() {
  if (g) {
    graph_free(g);
  }
}

// Helper function, fills graph
void fill_graph() {
  if (!init_with_file) {
    for (int i = 0; i < num_e; i++) {
      TEST_ASSERT_TRUE(graph_add_edge(g, connections[i][0], connections[i][1]));
    }
  }
}

// Tests
// Tests creation of a graph with 0 vertices.
void test_empty_graph_fails() {
  TEST_ASSERT_NULL(graph_init(0, UNDIRECTED));
}

// Tests number of vertices is correct.
void test_vertex_count() {
  TEST_ASSERT_EQUAL(num_v, graph_V(g));
}

// Tests number of edges is correct.
void test_edge_count() {
  if (!init_with_file) {
    TEST_ASSERT_EQUAL(0, graph_E(g));
    fill_graph();
  }
  TEST_ASSERT_EQUAL(num_e, graph_E(g));
}

// Test adjacent iterator failure
void test_adj_iter_empty() {
  if (!init_with_file) {
    TEST_ASSERT_FALSE(graph_adj_iter_init(g, 0));
    TEST_ASSERT_FALSE(graph_adj_iter_init(g, 13));
  }
}

// Test adjacent iterator. Note that the order of values assumes the underlying 
// implementation uses a linked adjacency list for each vertex.
void test_adj_iter() {
  fill_graph();

  int v = 0;
  int w;
  int expected[] = {6, 2, 1, 5};
  graph_adj_iter_init(g, v);
  int i = 0;
  while (graph_adj_iter_has_next(g, v)) {
    TEST_ASSERT_TRUE(graph_adj_iter_next(g, v, &w));
    TEST_ASSERT_EQUAL(expected[i], w);
    i++;
  }

  v = 5;
  int expected2[] = {3, 4, 0};
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
  int expected[] = {6, 2, 1, 5};
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
  if (!init_with_file) {
    int v = 0;
    TEST_ASSERT_FALSE(graph_adj_iter_init(g, v));
  }
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

// Test that trying to reverse an undirected graph fails.
void test_reverse_undirected() {
  TEST_ASSERT_NULL(graph_reverse(g));
}

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
    RUN_TEST(test_adj_iter_unconnected);
    RUN_TEST(test_adj_iter_out_of_bounds);
    RUN_TEST(test_add_edge_out_of_bounds);
    RUN_TEST(test_print);
    RUN_TEST(test_reverse_undirected);
  }

  return UNITY_END();
}
