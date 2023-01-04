#include "../../../Unity/src/unity.h"
#include "../substring_search/substring_search.h"
#include <string.h>
#include <stdlib.h>

static char *text1 = "INAHAYSTACKNEEDLEINAHAYSTACK";
static char *pattern1 = "NEEDLE";
static int found1 = 11;

static char *text2 = "BCBAABACAABABACAA";
static char *pattern2 = "ABABAC";
static int found2 = 9;

static char *text3 = "AABRAACADABRAACAADABRA";
static char *pattern3 = "AACAA";
static int found3 = 12;

static char *text4 = "ABRACADABRAABRACADABRAABRACADABRA";
static char *pattern4 = "ABRACADABRA";
static int found4 = 0;

int search_types[] = {BRUTE, KMP, BOYER_MOORE, RABIN_KARP};
int type;

void setUp() {
}

void tearDown() {
}

// Tests

void test_search1() {
  TEST_ASSERT_EQUAL(found1, substring_search(pattern1, text1, type));
  TEST_ASSERT_EQUAL(-1, substring_search("nomatch", text1, type));
}

void test_search2() {
  TEST_ASSERT_EQUAL(found2, substring_search(pattern2, text2, type));
  TEST_ASSERT_EQUAL(-1, substring_search("nomatch", text2, type));
}

void test_search3() {
  TEST_ASSERT_EQUAL(found3, substring_search(pattern3, text3, type));
  TEST_ASSERT_EQUAL(-1, substring_search("nomatch", text3, type));
}

void test_search4() {
  TEST_ASSERT_EQUAL(found4, substring_search(pattern4, text4, type));
  TEST_ASSERT_EQUAL(-1, substring_search("nomatch", text4, type));
}

// Main
int main() {
  UNITY_BEGIN();
  for (int i = 0; i < NELEMS(search_types); i++) {
    type = search_types[i];
    RUN_TEST(test_search1);
    RUN_TEST(test_search2);
    RUN_TEST(test_search3);
    RUN_TEST(test_search4);
  }
  return UNITY_END();
}
