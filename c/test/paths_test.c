#include "../../../Unity/src/unity.h"
#include "../paths/paths.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Text file format:
// num vertices
// num edges
// v w
static const int num_v = 6;
static const int num_e = 8;
static const int connections[num_e][2] = {{0, 5}, {2, 4}, {2, 3}, {1, 2}, \
                                          {0, 1}, {3, 4}, {3, 5}, {0, 2}};

graph_t *g;
paths_t *p;
enum paths_type type = DFS;
enum paths_type types[] = {DFS, BFS};

void setUp() {
  g = graph_init(num_v);
  for (int i = 0; i < num_e; i++) {
    TEST_ASSERT_TRUE(graph_add_edge(g, connections[i][0], connections[i][1]));
  }
}

void tearDown() {
  graph_free(g);
  if (p) {
    paths_free(p);
  }
}

// Tests
// Tests creation of a paths struct with out-of-bounds vertices
void test_out_of_bounds_vertex() {
  p = paths_init(g, -1, type);
  TEST_ASSERT_NULL(p);
  p = paths_init(g, num_v, type);
  TEST_ASSERT_NULL(p);
}

// Checks that the has_path_to responses are accurate
void test_has_path_to() {
  for (int s = 0; s < num_v; s++) {
    if (p) {
      paths_free(p);
    }
    p = paths_init(g, s, type);
    for (int v = 0; v < num_v; v++) {
      TEST_ASSERT_TRUE(has_path_to(p, v));
    }
    TEST_ASSERT_FALSE(has_path_to(p, -1));
    TEST_ASSERT_FALSE(has_path_to(p, num_v));
  }
}

// Test the path_to iterator (assumes iterator is implemented as a stack)
void test_paths_iter() {
  p = paths_init(g, 0, type);
  int path_dfs[] = {0, 2, 3, 5};
  int path_bfs[] = {0, 5};
  int v = 5;
  int w;
  TEST_ASSERT_TRUE(path_to_iter_init(p, v));
  int i = 0;
  while (path_to_iter_has_next(p, v)) {
    TEST_ASSERT_TRUE(path_to_iter_next(p, v, &w));
    if (type == DFS) {
      TEST_ASSERT_EQUAL(path_dfs[i], w);
    } else if (type == BFS) {
      TEST_ASSERT_EQUAL(path_bfs[i], w);
    }
    i++;
  }
}

// Test path_to iterator with repeated call on same vertex.
void test_paths_iter_repeat() {
  p = paths_init(g, 0, type);
  int path_dfs[] = {0, 2, 3, 5};
  int path_bfs[] = {0, 5};
  int v = 5;
  int w;
  TEST_ASSERT_TRUE(path_to_iter_init(p, v));
  int i = 0;
  while (path_to_iter_has_next(p, v)) {
    TEST_ASSERT_TRUE(path_to_iter_next(p, v, &w));
    if (type == DFS) {
      TEST_ASSERT_EQUAL(path_dfs[i], w);
    } else if (type == BFS) {
      TEST_ASSERT_EQUAL(path_bfs[i], w);
    }
    i++;
  }

  TEST_ASSERT_TRUE(path_to_iter_init(p, v));
  i = 0;
  while (path_to_iter_has_next(p, v)) {
    TEST_ASSERT_TRUE(path_to_iter_next(p, v, &w));
    if (type == DFS) {
      TEST_ASSERT_EQUAL(path_dfs[i], w);
    } else if (type == BFS) {
      TEST_ASSERT_EQUAL(path_bfs[i], w);
    }
    i++;
  }
}

// Tests the path_to iterator with out of bounds values.
void test_paths_iter_out_of_bounds() {
  p = paths_init(g, 0, type);
  TEST_ASSERT_FALSE(path_to_iter_init(p, -1));
  TEST_ASSERT_FALSE(path_to_iter_init(p, num_v));
  path_to_iter_init(p, 5);
  TEST_ASSERT_FALSE(path_to_iter_has_next(p, -1));
  TEST_ASSERT_FALSE(path_to_iter_has_next(p, num_v));
  
  int w;
  TEST_ASSERT_FALSE(path_to_iter_next(p, -1, &w));
  TEST_ASSERT_FALSE(path_to_iter_next(p, num_v, &w));
}

// Main
int main() {
  UNITY_BEGIN();
  
  for (int i = 0; i < NELEMS(types); i++) {
    type = types[i];
    RUN_TEST(test_out_of_bounds_vertex);
    RUN_TEST(test_has_path_to);
    RUN_TEST(test_paths_iter);
    RUN_TEST(test_paths_iter_repeat);
    RUN_TEST(test_paths_iter_out_of_bounds);
  }

  return UNITY_END();
}
