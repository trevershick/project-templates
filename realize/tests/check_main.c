#include <check.h>
#include <stdio.h>
#include <stdlib.h>

struct Suite *make_example_suite();

#define ADD_SUITE(runner, factory_method)                                      \
  {                                                                            \
    struct Suite *suite = factory_method();                                    \
    srunner_add_suite(runner, suite);                                          \
  }

int main(void) {
  struct Suite *s = suite_create(">");
  SRunner *sr = srunner_create(s);
  ADD_SUITE(sr, make_example_suite);
  srunner_set_tap(sr, "tests.tap");
  srunner_set_xml(sr, "tests.xml");
  srunner_run_all(sr, CK_VERBOSE /*CK_NORMAL*/);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
