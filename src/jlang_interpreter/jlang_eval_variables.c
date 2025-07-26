#include "jlang_eval_expressions.h"
#include "jlang_eval_function.h"
#include "../jlang_error.h"
#include "../vm.h"
#include "../stack.h"
#include "../jlang_token/jlang_token.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object_operations.h"
#include "../jlang_object/jlang_object_comparison.h"
#include "../jlang_object/jlang_object_string.h"
#include "../jlang_object/jlang_object_array.h"

void eval_variable_declarations(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  int is_const = syntax->token->type == CONST;
  syntax = syntax->left;
  while(syntax != NULL && syntax->token->type == IDENTIFIER)
  {
   if(jl_stack_get(vm_curr_frame(vm), syntax->token->literal) != NULL)
    {
      err_redeclaration(syntax->token);
      return;
    }
    jl_object_t *obj = eval_primary_expression(syntax->value, vm);
    obj->name = syntax->token->literal;
    stack_push(vm_curr_frame(vm), obj);
    syntax = syntax->left; 
  }
}

jl_object_t *eval_array_declaration(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *arr = jl_new_array();
  syntax = syntax->left;
  while(syntax != NULL)
  {
    jl_object_t *obj = eval_primary_expression(syntax, vm);
    jl_array_set(arr, arr->data.v_array->count, obj);
    syntax = syntax->left; 
  }
  
  return arr;
}

jl_object_t *eval_array_identifier(jl_syntax_t *syntax, vm_t *vm, jl_object_t *obj)
{
  if(syntax->right == NULL || syntax->right->token->type != LEFT_BRACKET)
  {
    return obj;
  }
  jl_object_t * index = eval_array_declaration(syntax->right, vm);
  if(index == NULL || index->data.v_array->count != 1)
  {
    err_expected_array_index(syntax->token);
    return NULL;
  }
  index = jl_array_get(index, 0);
  if(index->type != INT)
  {
    err_expected_array_index(syntax->token);
    return NULL;
  }
  return jl_array_get(obj, index->data.v_int);
}

jl_object_t *eval_identifier(jl_syntax_t *syntax, vm_t *vm)
{
  jl_object_t *obj = jl_stack_get(vm_curr_frame(vm), syntax->token->literal);
  if(obj == NULL)
  {
    err_identifier_does_not_exist(syntax->token);
    return NULL;
  }
  if(obj->type == ARRAY)
  {
    eval_array_identifier(syntax, vm, obj);
  }
  if(obj->type == FUNCTION_OBJECT)
  {
    if(syntax->left == NULL)
    {
      err_interpreter_error(syntax->token);
      return NULL;
    }
    ((jl_syntax_t *)obj->data.v_funct)->value = syntax->left;
    
    eval_function_identifier(obj->data.v_funct, vm);
  }
  return obj;
}
