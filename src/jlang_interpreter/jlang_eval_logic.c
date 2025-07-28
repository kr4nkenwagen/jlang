#include "jlang_interpreter.h"
#include "jlang_eval_expressions.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_error.h"
#include "../vm.h"
#include "../jlang_object/jlang_object.h"

void eval_while(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *condition = eval_primary_expression(syntax->value, vm);
  if(condition->type != BOOL_OBJECT)
  {
    err_expected_boolean_condition(syntax->token);
    return;
  }
  while(condition->data.v_bool == true)
  {
    interprete_branch(syntax, vm);
    condition = eval_primary_expression(syntax->value, vm);
  }
}

void eval_for(jl_syntax_t *syntax, vm_t *vm)
{

}

void eval_if(jl_syntax_t *syntax, vm_t *vm)
{
  while(
    syntax != NULL && (
    syntax->token->type == IF || 
    syntax->token->type == ELSE_IF || 
    syntax->token->type == ELSE))
  {
    jl_object_t *condition = eval_primary_expression(syntax->value, vm);
    if(syntax->token->type == ELSE || condition->data.v_bool == true)
    {
      interprete_branch(syntax, vm);
      return;
    }
    syntax = syntax->right;
  }
}


