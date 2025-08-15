#include "jlang_interpreter.h"
#include "jlang_eval_expressions.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_error.h"
#include "../vm.h"
#include "../jlang_object/jlang_object.h" 
#include "../stack.h"
#include "../jlang_program.h"

void eval_while(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *condition = eval_primary_expression(syntax->value, vm, program);
  if(condition->type != BOOL_OBJECT)
  {
    err_expected_boolean_condition(syntax->token);
    return;
  }
  while(condition->data.v_bool == true)
  {
    interprete_branch(syntax, vm);
    condition = eval_primary_expression(syntax->value, vm, program);
  }
}

void eval_for(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return;
  }
  vm_push_frame(vm, stack_new(), true);
  eval_primary_expression(syntax->left, vm, program);
  jl_object_t *condition = eval_primary_expression(syntax->value, vm, program);
  if(condition->type != BOOL_OBJECT)
  {
    err_expected_boolean_condition(syntax->token);
    return;
  }
  while(condition->data.v_bool == true)
  {
    interprete_branch(syntax, vm);
    eval_primary_expression(syntax->right, vm, program);
    condition = eval_primary_expression(syntax->value, vm, program);
  }
  vm_pop_frame(vm);
}

void eval_if(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  while(
    syntax != NULL && (
    syntax->token->type == IF || 
    syntax->token->type == ELSE_IF || 
    syntax->token->type == ELSE))
  {
    jl_object_t *condition = eval_primary_expression(syntax->value, vm, program);
    if(syntax->token->type == ELSE || condition->data.v_bool == true)
    {
      interprete_branch(syntax, vm);
      return;
    }
    syntax = syntax->right;
  }
}

void eval_break(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  jl_program_t *curr_prog = program;
  while(curr_prog->type != PROG_LOOP)
  {
    curr_prog->exit = true;
    curr_prog = curr_prog->parent;
  }
  curr_prog->exit = true;
}

void eval_continue(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  jl_program_t *curr_prog = program;
  while(curr_prog->type != PROG_LOOP)
  {
    curr_prog->exit = true;
    curr_prog = curr_prog->parent;
  }
  curr_prog->continueing = true;
}

void eval_return(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  jl_program_t *curr_prog = program;
  while(curr_prog->type != PROG_FUNCTION)
  {
    curr_prog->exit = true;
    curr_prog = curr_prog->parent;
  }
  curr_prog->exit = true;
  curr_prog->ret_value = eval_primary_expression(syntax->value, vm, program);
}
