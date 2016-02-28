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
#include "../neuro/system.h"
#include "../neuro/core.h"
#include "../neuro/wm.h"


//----------------------------------------------------------------------------------------------------------------------
// CORE SUITE
//----------------------------------------------------------------------------------------------------------------------

int init_core_suite(void) {
  if (!NeuroCoreInit())
    return -1;
  return 0;
}

int clean_core_suite(void) {
  NeuroCoreStop();
  return 0;
}


//----------------------------------------------------------------------------------------------------------------------
// CORE TESTS
//----------------------------------------------------------------------------------------------------------------------

void add_remove_client(void) {
  // Create a fake client
  const Window win = 0UL;
  NeuroClient *const cli = NeuroTypeNewClient(win, NULL);
  CU_ASSERT_PTR_NOT_NULL(cli);

  // Add it to the stack list
  NeuroClientPtrPtr c = NeuroCoreAddClientStart(cli);
  CU_ASSERT_PTR_NOT_NULL(c);

  // Remove it from the stack list
  NeuroClient *const cli2 = NeuroCoreRemoveClient(c);
  CU_ASSERT_PTR_NOT_NULL(cli2);
  CU_ASSERT(cli == cli2);

  // Free the fake client
  NeuroTypeDeleteClient(cli2);
}

void set_curr_stack(void) {
  NeuroCoreSetCurrStack(1);
  CU_ASSERT(NeuroCoreGetCurrStack() == 1);
}


//----------------------------------------------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------------------------------------------

int main() {
  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Add a suite to the registry
  CU_pSuite core_suite = CU_add_suite("Core_Suite", init_core_suite, clean_core_suite);
  if (NULL == core_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add the tests to the suite
  if ((NULL == CU_add_test(core_suite, "add_remove_client()", add_remove_client)) ||
      (NULL == CU_add_test(core_suite, "set_curr_stack()", set_curr_stack))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}


