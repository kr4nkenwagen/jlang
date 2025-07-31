#include <stdio.h>
#include "jlang_syntax/jlang_syntax.h"
#include "jlang_object/jlang_object.h"

typedef struct jl_program {
  bool exit;
  jl_syntax_t **statements;
  size_t size;
  int count;
  jl_object_t *ret_value;
} jl_program_t;

jl_program_t *jl_new_program();
void jl_program_add(jl_program_t *program, jl_syntax_t *statement);
