#include "../../../Unity/src/unity.h"
#include "../connected_components/connected_components.h"
#include "../graph/graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Text file format:
// num vertices
// num edges
// v w
static const int num_v = 13;
static const int num_e = 13;
static const int connections[num_e][2] = {{0, 5}, {4, 3}, {0, 1}, {9, 12}, \
                                              {6, 4}, {5, 4}, {0, 2}, {11, 12},\
                                              {9, 10}, {0, 6}, {7, 8}, {9, 11},\
                                              {5, 3}};

void setUp() {
}

void tearDown() {
}

// Helper function
void fill_graph(graph_t *g) {
  for (int i = 0; i < num_e; i++) {
    graph_add_edge(g, connections[i][0], connections[i][1]);
  }
}

// Tests

void test_undirected_cc() {
  graph_t *g = graph_init(num_v, UNDIRECTED);
  fill_graph(g);

  cc_t *cc = cc_init(g);
  
  TEST_ASSERT_EQUAL(3, cc_count(cc));
  
  int expected[] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2};

  for (int i = 0; i < graph_V(g); i++) {
    TEST_ASSERT_EQUAL(expected[i], cc_id(cc, i));
  }

  TEST_ASSERT_TRUE(cc_connected(cc, 0, 1));
  TEST_ASSERT_TRUE(cc_connected(cc, 7, 8));
  TEST_ASSERT_TRUE(cc_connected(cc, 9, 10));
  TEST_ASSERT_FALSE(cc_connected(cc, 8, 9));

  cc_free(cc);
}

void test_directed_scc() {
  graph_t *g = graph_init(13, DIRECTED);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 0, 5);
  graph_add_edge(g, 2, 0);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 3, 2);
  graph_add_edge(g, 3, 5);
  graph_add_edge(g, 4, 2);
  graph_add_edge(g, 4, 3);
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

  cc_t *cc = cc_init(g);
  
  TEST_ASSERT_EQUAL(5, cc_count(cc));
  
  int expected[] = {1, 0, 1, 1, 1, 1, 3, 4, 3, 2, 2, 2, 2};

  for (int i = 0; i < graph_V(g); i++) {
    TEST_ASSERT_EQUAL(expected[i], cc_id(cc, i));
  }

  TEST_ASSERT_TRUE(cc_connected(cc, 0, 2));
  TEST_ASSERT_TRUE(cc_connected(cc, 10, 12));
  TEST_ASSERT_TRUE(cc_connected(cc, 6, 8));
  TEST_ASSERT_FALSE(cc_connected(cc, 1, 0));
  cc_free(cc);
}

// Main
int main() {
  UNITY_BEGIN();
  
  RUN_TEST(test_undirected_cc);
  RUN_TEST(test_directed_scc);

  return UNITY_END();
}