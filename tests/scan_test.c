#include <CUnit/CUnit.h>
#include "../src/scan.h" 


void open_src_test()
{
  CU_ASSERT(open_src(NULL) == NULL);
  source_code_t *src = open_src("tests/test.jonx");
  CU_ASSERT(src != NULL);
  CU_ASSERT(src->is_at_end == 0);
  CU_ASSERT(src->pointer == -1);
  CU_ASSERT(src->line == 0);
  CU_ASSERT(src->column == 0);
  CU_ASSERT(src->size == 207);
  CU_ASSERT(strlen(src->src) == 207);
  free(src);
}

void jl_source_code_advance_test()
{
  source_code_t *src = open_src("tests/test.jonx");
  CU_ASSERT(src != NULL);
  for(int i = 0; i < 26; i++)
  {
    CU_ASSERT(jl_source_code_advance(src) != 0);
  }
  CU_ASSERT(src->pointer == 25);
  CU_ASSERT(src->line == 1);
  CU_ASSERT(src->column == 7);
  src->pointer = 207;
  jl_source_code_advance(src);
  CU_ASSERT(src->is_at_end == 1);
  CU_ASSERT(jl_source_code_advance(src) == 0);
  free(src);
}

void peek_test()
{
  source_code_t *src = open_src("tests/test.jonx");
  src->pointer = 2;
  CU_ASSERT(peek(src, -1) == 'T');
  CU_ASSERT(peek(src, 0) == 'h');
  CU_ASSERT(peek(src, 1) == 'i');
  CU_ASSERT(peek(src, 207) == 0);
  CU_ASSERT(peek(NULL, 0) == 0);
  free(src);
}
