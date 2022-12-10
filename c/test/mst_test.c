#include "../../../Unity/src/unity.h"
#include "../mst/mst.h"
#include "../edge/edge.h"
#include "../weighted_graph/weighted_graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum mst_type type = PRIM;
enum mst_type types[] = {PRIM, KRUSKAL};

void setUp() {
}

void tearDown() {
}

// Tests

void test_mst() {
  weighted_graph_t *g = weighted_graph_init_file("../data/tinyEWG.txt", UNDIRECTED);
  mst_t *mst = mst_init(g, type);
  TEST_ASSERT_NOT_NULL(mst);

  TEST_ASSERT_EQUAL_DOUBLE(1.81, mst_weight(mst));

  TEST_ASSERT_TRUE(mst_iter_init(mst));

  edge_t *e;
  while (mst_iter_has_next(mst)) {
    TEST_ASSERT_TRUE(mst_iter_next(mst, &e));
    edge_print(e);
  }
}

// Main
int main() {
  UNITY_BEGIN();
  
  for (int i = 0; i < NELEMS(types); i++) {
    type = types[i];
    RUN_TEST(test_mst);
  }

  return UNITY_END();
}
