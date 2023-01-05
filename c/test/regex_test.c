#include "../../../Unity/src/unity.h"
#include "../regex/regex.h"
#include <string.h>
#include <stdlib.h>

void setUp() {
}

void tearDown() {
}


void test_empty_re() {
  regex_t *re = regex_init("");
  TEST_ASSERT_NULL(re);
}

void test_mismatched_parens_re() {
  regex_t *re = regex_init("(.*NEEDLE.*");
  TEST_ASSERT_NULL(re);

  re = regex_init(".*NEEDLE.*)");
  TEST_ASSERT_NULL(re);
}

void test_substring_re() {
  regex_t *re = regex_init(".*NEEDLE.*");
  TEST_ASSERT_NOT_NULL(re);

  TEST_ASSERT_TRUE(regex_recognizes(re, "A HAYSTACK NEEDLE IN"));
  TEST_ASSERT_TRUE(regex_recognizes(re, "NEEDLE IN"));
  TEST_ASSERT_TRUE(regex_recognizes(re, "NEEDLE"));
  TEST_ASSERT_TRUE(regex_recognizes(re, "NEEDLE "));
  TEST_ASSERT_TRUE(regex_recognizes(re, " NEEDLE"));
  TEST_ASSERT_TRUE(regex_recognizes(re, "A HAYSTACK NEEDLE"));
  TEST_ASSERT_FALSE(regex_recognizes(re, ""));
  TEST_ASSERT_FALSE(regex_recognizes(re, "A HAYSTACK NEE_DLE IN"));
  TEST_ASSERT_FALSE(regex_recognizes(re, "A HAYSTACK NEEDL"));

  regex_free(re);
}

void test_genome_re() {
  regex_t *re = regex_init("gcg(cgg|agg)*ctg");
  TEST_ASSERT_NOT_NULL(re);
  
  TEST_ASSERT_TRUE(regex_recognizes(re, "gcgaggaggcggcggctg"));
  TEST_ASSERT_TRUE(regex_recognizes(re, "gcgctg"));
  TEST_ASSERT_FALSE(regex_recognizes(re, "gcgagcctg"));
  TEST_ASSERT_FALSE(regex_recognizes(re, "gcgaggaggcggcggct"));

  regex_free(re);
}

void test_cascaded_or_re() {
  regex_t *re = regex_init("ABC(A|B|C)");
  TEST_ASSERT_NULL(re);

  re = regex_init("ABC((A|B)|C)");
  TEST_ASSERT_NOT_NULL(re);
  regex_free(re);
}

void test_disallowed_characters_re() {
  regex_t *re = regex_init("(AB){3}");
  TEST_ASSERT_NULL(re);

  re = regex_init("(AB)+");
  TEST_ASSERT_NULL(re);

  re = regex_init("(AB)?");
  TEST_ASSERT_NULL(re);

  re = regex_init("[A-Z]");
  TEST_ASSERT_NULL(re);
}

void test_ABCD_re() {
  regex_t *re = regex_init("((A*B|AC)D)");
  TEST_ASSERT_NOT_NULL(re);
  
  TEST_ASSERT_TRUE(regex_recognizes(re, "AAAABD"));

  regex_free(re);
}

// Main
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_empty_re);
  RUN_TEST(test_mismatched_parens_re);
  RUN_TEST(test_substring_re);
  RUN_TEST(test_genome_re);
  RUN_TEST(test_ABCD_re);
  RUN_TEST(test_cascaded_or_re);
  RUN_TEST(test_disallowed_characters_re);
  return UNITY_END();
}
