//----------------------------------------------------------------------------------------------------------------------
// Program     :  cunit_test
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../neuro/stackset.h"
#include "../neuro/neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// STACKSET SUITE
//----------------------------------------------------------------------------------------------------------------------
int init_stackset_suite(void) {
  setConfigS(&defWMConfig);
  if (!initSS())
    return -1;
  return 0;
}

int clean_stackset_suite(void) {
  endSS();
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
// STACKSET TESTS
//----------------------------------------------------------------------------------------------------------------------


void add_remove_client(void) {
  // Create a fake client
  const Window w = 0;
  const XWindowAttributes wa = (XWindowAttributes){.x = 0, .y = 0, .width = 500, .height = 500};
  Client *cli = allocClientT(w, &wa);
  CU_ASSERT_PTR_NOT_NULL(cli);

  // Add it to the stackset
  CliPtr c = addClientStartSS(cli);
  CU_ASSERT_PTR_NOT_NULL(c);

  // Remove it from the stackset
  Client *cli2 = rmvClientSS(c);
  CU_ASSERT_PTR_NOT_NULL(cli2);
  CU_ASSERT(cli == cli2);

  // Free the fake client
  freeClientT(cli2);
}

void set_curr_stack(void) {
  setCurrStackSS(1);
  CU_ASSERT(getCurrStackSS() == 1);
}


//----------------------------------------------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------------------------------------------

int main() {
  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Add a suite to the registry
  CU_pSuite stackset_suite = CU_add_suite("StackSet_Suite", init_stackset_suite, clean_stackset_suite);
  if (NULL == stackset_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add the tests to the suite
  if ((NULL == CU_add_test(stackset_suite, "add_remove_client()", add_remove_client)) ||
      (NULL == CU_add_test(stackset_suite, "set_curr_stack()", set_curr_stack))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}


