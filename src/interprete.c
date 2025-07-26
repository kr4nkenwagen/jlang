#include <string.h>
#include "jlang_object.h"
#include "jlang_syntax.h"
#include "jlang_token.h"
#include "jlang_program.h"
#include "jlang_common.h"
#include "jlang_error.h"
#include "vm.h"
#include "stack.h"
#include "stack.h"

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm);

jl_object_t *interprete(jl_program_t *program, vm_t *vm)
{
  if(program == NULL)
  {
    return NULL;
  }
  for(int i = 0; i < program->count; i++)
  {
    jl_print_object(eval_primary_expression(program->statements[i], vm));
  }
}

jl_object_t *eval_number(jl_syntax_t *syntax)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  if(strchr(syntax->token->literal, '.') != NULL)
  {
    return jl_new_float(atof(syntax->token->literal));
  }
  else 
  {
    return jl_new_int(atoi(syntax->token->literal));
  }
  return NULL;
}
jl_object_t *eval_string_operation_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);
  if(syntax->token->type == COLON)
  {
    if(right_hand_side->type == INT)
    {
      return jl_substring(left_hand_side, 0, right_hand_side->data.v_int);
    }
    if(right_hand_side->type == STRING_WRAPPER)
    {
      int position = jl_position_of_first_instance(left_hand_side, right_hand_side->data.v_string);
      if(position == -1)
      {
        return NULL;
      }
      return jl_substring(left_hand_side, 0, position);
    }
  }
  else if(syntax->token->type == COLON_HAT)
  {
    if(right_hand_side->type == INT)
    {
      size_t size = jl_length(left_hand_side);
      return jl_substring(left_hand_side, size - right_hand_side->data.v_int, right_hand_side->data.v_int);
    }
    if(right_hand_side->type == STRING_WRAPPER)
    {
      int position = jl_position_of_last_instance(left_hand_side, right_hand_side->data.v_string);
      if(position == -1)
      {
        return NULL;
      }
      return jl_substring(left_hand_side, position + 1, -1);
    }
  }
  else if(syntax->token->type == DOT_DOT)
  {
    if(left_hand_side->type == STRING_WRAPPER || right_hand_side->type == STRING_WRAPPER)
    {
      return jl_add(left_hand_side, right_hand_side);
    }
    err_illegal_operation(syntax->token);
  }
}

jl_object_t *eval_unary_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);
  if(right_hand_side != NULL || right_hand_side->type == BOOLEAN)
  {
    return jl_new_bool(!right_hand_side->data.v_bool);
  }
  err_illegal_operation(syntax->token);
}

jl_object_t *eval_comparison_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);
  if(syntax->token->type == EQUAL_EQUAL)
  {
    return jl_equals(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == BANG_EQUAL)
  {
    return jl_not_equals(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == GREATER_EQUAL)
  {
    return jl_greater_equals(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == LESS_EQUAL)
  {
    return jl_less_equals(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == LESS)
  {
    return jl_less(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == GREATER)
  {
    return jl_greater(left_hand_side, right_hand_side);
  }
  return NULL;
}

bool divide_by_zero(jl_object_t *a, jl_object_t *b)
{
  if((a->type == INT && a->data.v_int == 0) ||
   (a->type == FLOAT && a->data.v_float == 0) ||
   (b->type == INT && b->data.v_int == 0) ||
   (b->type == FLOAT && b->data.v_float == 0))
  {
    return true;
  }  
  return false;
}

jl_object_t *eval_binary_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);

  if(syntax->token->type == PLUS)
  {
    return jl_add(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == MINUS)
  {
    return jl_subtract(left_hand_side, right_hand_side);
  }
  else if(syntax->token->type == STAR)
  {
    return jl_multiply(left_hand_side, right_hand_side);
  }
  if(divide_by_zero(left_hand_side, right_hand_side))
  {
    err_divide_by_zero(syntax->token);
  }
  if(syntax->token->type == SLASH)
  {
    return jl_divide(left_hand_side, right_hand_side);
  }
   else if(syntax->token->type == MODULUS)
  {
    return jl_modulus(left_hand_side, right_hand_side);
  }
  return NULL;
}

void eval_assignment_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);
  switch(syntax->token->type)
  {
    case EQUAL:
      jl_assign(left_hand_side, right_hand_side);
    break;
    case PLUS_EQUAL:
      jl_assign(left_hand_side, jl_add(left_hand_side, right_hand_side));
    break;
    case MINUS_EQUAL:
      jl_assign(left_hand_side, jl_subtract(left_hand_side, right_hand_side));
    break;
    case STAR_EQUAL:
      jl_assign(left_hand_side, jl_multiply(left_hand_side, right_hand_side));
    break;
    case SLASH_EQUAL:
      if(divide_by_zero(left_hand_side, right_hand_side))
      {
        err_divide_by_zero(syntax->token);
      }
      jl_assign(left_hand_side, jl_divide(left_hand_side, right_hand_side));
    break;
  }
}

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

void interprete_branch(jl_syntax_t *syntax, vm_t *vm)
{
  vm_push_frame(vm, stack_new());
  if(syntax->args !=NULL)
  {
    interprete(syntax->args, vm);
    jl_object_t *arg_vals = eval_array_declaration(syntax->value, vm);
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
}

void eval_while(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *condition = eval_primary_expression(syntax->value, vm);
  if(condition->type != BOOLEAN)
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

jl_object_t *eval_function_identifier(jl_syntax_t *syntax, vm_t *vm)
{
  interprete_branch(syntax, vm);
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

void eval_function_declaration(jl_syntax_t *syntax, vm_t *vm)
{
  jl_object_t *funct = jl_new_funct(syntax->right);
  funct->name = syntax->right->token->literal;
  stack_push(vm_curr_frame(vm), funct);
}

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  switch(syntax->token->type)
  {
    case FUNCTION:
      eval_function_declaration(syntax, vm);
      return NULL;
    case IF:
      eval_if(syntax, vm);
      return NULL;
    case LEFT_BRACKET:
      return eval_array_declaration(syntax, vm);
    case WHILE:
      eval_while(syntax, vm);
      return NULL;
    case COLON:
    case COLON_HAT:
    case DOT_DOT:
      return eval_string_operation_expression(syntax, vm);
    case BANG:
      return eval_unary_expression(syntax, vm);
    case EQUAL_EQUAL:
    case BANG_EQUAL:
    case GREATER_EQUAL:
    case LESS_EQUAL:
    case LESS:
    case GREATER:
      return eval_comparison_expression(syntax, vm);
    case EQUAL:
    case PLUS_EQUAL:
    case MINUS_EQUAL:
    case STAR_EQUAL:
    case SLASH_EQUAL:
      eval_assignment_expression(syntax, vm);
      return NULL;
    case CONST:
    case VAR:
      eval_variable_declarations(syntax, vm);
      return NULL;
    case IDENTIFIER:
      return eval_identifier(syntax, vm);
    case STRING_WRAPPER:
      return jl_new_string(syntax->token->literal);
    case NUMBER:
      return eval_number(syntax);
    case NIL:
      return jl_new_null();
    case TRUE:
      return jl_new_bool(true);
    case FALSE:
      return jl_new_bool(false);
    case PLUS:
    case MINUS:
    case STAR:
    case SLASH:
    case MODULUS: 
      return eval_binary_expression(syntax, vm);
    default:
      err_interpreter_error(syntax->token);
    break;
  }
  return NULL;
}
