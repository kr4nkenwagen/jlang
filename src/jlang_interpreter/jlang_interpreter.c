#include <string.h>
#include "jlang_eval_logic.h"
#include "jlang_eval_variables.h"
#include "jlang_eval_function.h"
#include "jlang_eval_expressions.h"
#include "../jlang_program.h"
#include "../debug.h"
#include "../jlang_error.h"
#include "../vm.h"
#include "../stack.h"
#include "../jlang_token/jlang_token.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object_operations.h"
#include "../jlang_object/jlang_object_comparison.h"
#include "../jlang_object/jlang_object_string.h"
#include "../jlang_object/jlang_object_array.h"


jl_object_t *interprete(jl_program_t *program, vm_t *vm)
{
  if(program == NULL)
  {
    return NULL;
  }
  for(int i = 0; i < program->count; i++)
  {
    if(program->exit)
    {
      return program->ret_value;
    }
    jl_print_object(eval_primary_expression(program->statements[i], vm, program));
  }
}
jl_object_t *interprete_branch(jl_syntax_t *syntax, vm_t *vm)
{
  bool is_function = syntax->token->type == FUNCTION;
  vm_push_frame(vm, stack_new(), !is_function);
  if(syntax->args != NULL)
  {
    interprete(syntax->args, vm);
    jl_object_t *arg_vals = eval_array_declaration(syntax->value, vm, syntax->branch);
    stack_t * curr_stack = vm_curr_frame(vm);
    if(curr_stack->count - curr_stack->parent_references != arg_vals->data.v_array->count)
    {
      err_incorrect_number_of_references(syntax->token);
    }
    for(int i = curr_stack->parent_references; i < curr_stack->count; i++)
    {
      curr_stack->data[i]->data = arg_vals->data.v_array->elements[i - curr_stack->parent_references]->data;
      curr_stack->data[i]->type = arg_vals->data.v_array->elements[i - curr_stack->parent_references]->type;
    }
  }
  interprete(syntax->branch, vm);
  vm_pop_frame(vm);
  if(is_function)
  {
    return ((jl_program_t *)syntax->branch)->ret_value;
  }
}

