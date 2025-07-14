#include <stdio.h>
#include "jlang_syntax.h"

typedef struct jl_program {
  jl_syntax_t **statements;
  size_t size;
  int count;
} jl_program_t;

jl_program_t *jl_new_program();
void jl_program_add(jl_program_t *program, jl_syntax_t *statement);
