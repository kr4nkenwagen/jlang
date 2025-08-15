#include <stdio.h>
#include "jlang_syntax/jlang_syntax.h"
#include "jlang_object/jlang_object.h"

typedef enum jl_program_type {
  PROG_SOURCE,
  PROG_LOOP,
  PROG_FUNCTION,
  PROG_IF
}jl_program_type_t;

typedef struct jl_program jl_program_t;

typedef struct jl_program {
  bool exit;
  bool breaking;
  bool continueing; 
  int pointer;
  jl_program_type_t type;
  jl_syntax_t **statements;
  size_t size;
  int count;
  jl_object_t *ret_value;
  jl_program_t *parent;
} jl_program_t;

jl_program_t *jl_new_program(jl_program_t *parent);
void jl_program_add(jl_program_t *program, jl_syntax_t *statement);
