#include <stdio.h>
#include <string.h>
#include "jlang_program.h"
#include "jlang_syntax/jlang_syntax.h"

jl_program_t *jl_new_program()
{
  jl_program_t *program = malloc(sizeof(jl_program_t));
  if(program == NULL)
  {
    return NULL;
  }
  program->size = 8;
  program->count = 0;
  program->exit = false;
  program->statements = malloc(sizeof(jl_syntax_t) * program->size);
  program->ret_value = NULL;
  return program;
}

void jl_program_add(jl_program_t *program, jl_syntax_t *statement)
{
  if(statement == NULL)
  {
    return;
  }
  program->statements[program->count++] = statement;
  if(program->count == program->size)
  {
    jl_syntax_t **tmp_statements = malloc(sizeof(jl_syntax_t) * program->size);
    if(tmp_statements == NULL)
    {
      return;
    }
    memcpy(tmp_statements, program->statements, sizeof(jl_syntax_t) * program->size);
    program->size *= 2;
    program->statements = malloc(sizeof(jl_syntax_t) * program->size);
    if(program->statements == NULL)
    {
      return;
    }
    memcpy(program->statements, tmp_statements, sizeof(jl_syntax_t) * program->size / 2);
  }
}
