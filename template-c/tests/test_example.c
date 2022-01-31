#include <check.h>
#include <check_stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "check_macros.h"

START_TEST(test_basics1) { ck_assert_uint_eq(1, 1); }
END_TEST

START_TEST(test_others1) { ck_assert_uint_eq(1, 1); }
END_TEST

// clang-format off
CK_SUITE(example,
  CK_CASE(basics,
    CK_TEST(test_basics1)
  )
  CK_CASE(others,
    CK_TEST(test_others1)
  )
)
// clang-format on
