#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "scan_test.h"
#include "jlang_object_test.h"


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

  CU_pSuite jlang_object_suite = CU_add_suite("JLangObjectTests", 0, 0);
  if(jlang_object_suite == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(CU_add_test(jlang_object_suite, "jl_new_int() tests", jl_new_int_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_new_float() tests", jl_new_float_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_new_string() tests", jl_new_string_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_new_array() tests", jl_new_array_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_new_array() tests", jl_new_array_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_object_free() tests", jl_object_free_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_length() tests", jl_length_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_add() tests", jl_add_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_array_set() tests", jl_array_set_test) == NULL ||
     CU_add_test(jlang_object_suite, "jl_array_get() tests", jl_array_get_test) == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
