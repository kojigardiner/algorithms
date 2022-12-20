#include "../../../Unity/src/unity.h"
#include "../flow_network/flow_network.h"
#include "../flow_edge/flow_edge.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

flow_network_t *f;

void setUp() {
}

void tearDown() {
}

// Helper method
void fill_network() {
  f = flow_network_init(6);

  flow_edge_t *e;

  e = flow_edge_init(0, 1, 2.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(0, 2, 3.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(1, 3, 3.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(1, 4, 1.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(2, 3, 1.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(2, 4, 1.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(3, 5, 2.0);
  flow_network_add_edge(f, e);

  e = flow_edge_init(4, 5, 3.0);
  flow_network_add_edge(f, e);
}

// Tests
void test_flow_edge() {
  flow_edge_t *e = flow_edge_init(3, 5, 2.0);
  TEST_ASSERT_NOT_NULL(e);
  
  TEST_ASSERT_EQUAL(flow_edge_from(e), 3);
  TEST_ASSERT_EQUAL(flow_edge_to(e), 5);
  TEST_ASSERT_EQUAL(flow_edge_other(e, 3), 5);
  TEST_ASSERT_EQUAL(flow_edge_other(e, 5), 3);
  TEST_ASSERT_EQUAL(flow_edge_capacity(e), 2.0);
  TEST_ASSERT_EQUAL(flow_edge_flow(e), 0);
  
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 3), 0.0);
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 5), 2.0);
  flow_edge_add_residual_flow_to(e, 5, 2.0);
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 3), 2.0);
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 5), 0.0);

  flow_edge_add_residual_flow_to(e, 3, 1.0);
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 3), 1.0);
  TEST_ASSERT_EQUAL_DOUBLE(flow_edge_residual_capacity_to(e, 5), 1.0);

  flow_edge_free(e);
}

void test_flow_network() {
  fill_network();
   
  TEST_ASSERT_NOT_NULL(f);

  TEST_ASSERT_EQUAL(flow_network_V(f), 6);
  TEST_ASSERT_EQUAL(flow_network_E(f), 8);

  flow_edge_t *e;
  flow_network_adj_iter_init(f, 2);
  int expected_v[] = {2, 2, 0};
  int expected_w[] = {4, 3, 2};
  double expected_capacity[] = {1.0, 1.0, 3.0};
  double expected_flow[] = {0.0, 0.0, 0.0};
  int i = 0;
  while (flow_network_adj_iter_has_next(f, 2)) {
    flow_network_adj_iter_next(f, 2, &e);
    TEST_ASSERT_EQUAL(flow_edge_from(e), expected_v[i]);
    TEST_ASSERT_EQUAL(flow_edge_to(e), expected_w[i]);
    TEST_ASSERT_EQUAL_DOUBLE(flow_edge_capacity(e), expected_capacity[i]);
    TEST_ASSERT_EQUAL_DOUBLE(flow_edge_flow(e), expected_flow[i]);
    i++;
  }

  flow_network_edge_iter_init(f);
  while (flow_network_edge_iter_has_next(f)) {
    flow_network_edge_iter_next(f, &e);
    flow_edge_print(e);
  }

  flow_network_free(f);
}

void test_maxflow() {
  fill_network();
  TEST_ASSERT_EQUAL_DOUBLE(flow_network_compute_maxflow(f, 0, 5), 4.0);
  flow_network_print(f);

  // Compute again, to make sure the internal data structures are still intact.
  TEST_ASSERT_EQUAL_DOUBLE(flow_network_compute_maxflow(f, 0, 5), 4.0);
}

void test_mincut() {
  bool expected[] = {true, false, true, false, false, false};

  fill_network();
  for (int i = 0; i < flow_network_V(f); i++) {
    TEST_ASSERT_EQUAL(expected[i], flow_network_incut(f, 0, 5, i));
  }
}

// Main
int main() {
  UNITY_BEGIN();

  RUN_TEST(test_flow_edge);
  RUN_TEST(test_flow_network);
  RUN_TEST(test_maxflow);
  RUN_TEST(test_mincut);

  return UNITY_END();
}
