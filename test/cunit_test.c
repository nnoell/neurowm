#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "../neuro/neurowm.h"

int init_suite1(void) {
  return 0;
}

int clean_suite1(void) {
  return 0;
}

// Test 1
void test1(void) {
  CU_ASSERT(0 == 0);
}

// Test 2
void test2(void) {
  CU_ASSERT(1 == 1);
}

// Main
int main() {
  CU_pSuite pSuite = NULL;

  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Add a suite to the registry
  pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add the tests to the suite
  if ((NULL == CU_add_test(pSuite, "test of test1()", test1)) ||
      (NULL == CU_add_test(pSuite, "test of test2()", test2))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}


