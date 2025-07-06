#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "scan_test.h"


int main()
{
  if(CU_initialize_registry() != CUE_SUCCESS)
  {
    return CU_get_error();
  }
  CU_pSuite scan_suite = CU_add_suite("ScanTests", 0, 0);
  if(scan_suite == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(CU_add_test(scan_suite, "open_src() tests", open_src_test) == NULL ||
     CU_add_test(scan_suite, "advance() tests", advance_test) == NULL ||
     CU_add_test(scan_suite, "peek() tests", peek_test) == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
