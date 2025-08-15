#include <string.h>
#include "jlang_interpreter.h"
#include "jlang_eval_variables.h"
#include "jlang_eval_logic.h"
#include "jlang_eval_function.h"
#include "../jlang_predefined_functions/jlang_print.h"
#include "../jlang_predefined_functions/jlang_println.h"
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
#include "../jlang_program.h"

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);

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

jl_object_t *eval_string_operation_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm, program);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm, program);
  if(syntax->token->type == COLON)
  {
    if(right_hand_side->type == INT_OBJECT)
    {
      return jl_substring(left_hand_side, 0, right_hand_side->data.v_int);
    }
    if(right_hand_side->type == STRING_OBJECT)
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
    if(right_hand_side->type == INT_OBJECT)
    {
      size_t size = jl_length(left_hand_side);
      return jl_substring(left_hand_side, size - right_hand_side->data.v_int, right_hand_side->data.v_int);
    }
    if(right_hand_side->type == STRING_OBJECT)
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
    if(left_hand_side->type == STRING_OBJECT || right_hand_side->type == STRING_OBJECT)
    {
      return jl_add(left_hand_side, right_hand_side);
    }
    err_illegal_operation(syntax->token);
  }
}

jl_object_t *eval_unary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm, program);
  if(right_hand_side != NULL || right_hand_side->type == BOOL_OBJECT)
  {
    return jl_new_bool(!right_hand_side->data.v_bool);
  }
  err_illegal_operation(syntax->token);
}

jl_object_t *eval_comparison_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm, program);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm, program);
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
  if((a->type == INT_OBJECT && a->data.v_int == 0) ||
   (a->type == FLOAT_OBJECT && a->data.v_float == 0) ||
   (b->type == INT_OBJECT && b->data.v_int == 0) ||
   (b->type == FLOAT_OBJECT && b->data.v_float == 0))
  {
    return true;
  }  
  return false;
}

jl_object_t *eval_binary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm, program);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm, program);
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

void eval_assignment_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm, program);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm, program);
  if(left_hand_side->is_const)
  {
    err_modify_constant(syntax->token);
    return;
  }
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

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  switch(syntax->token->type)
  {   
    case CONTINUE:
      eval_continue(syntax, vm, program);
      return NULL;
    case BREAK:
      eval_break(syntax, vm, program);
      return NULL;
    case RETURN:
      eval_return(syntax, vm, program);
      return NULL;
    case PRINT_LINE:
      jl_println(eval_primary_expression(syntax->value, vm, program));
      return NULL;
    case FOR:
      eval_for(syntax, vm, program);
      return NULL;
    case PRINT: 
      jl_print(eval_primary_expression(syntax->value, vm, program));
      return NULL;
    case FUNCTION:
      eval_function_declaration(syntax, vm, program);
      return NULL;
    case IF:
      eval_if(syntax, vm, program);
      return NULL;
    case LEFT_BRACKET:
      return eval_array_declaration(syntax, vm, program);
    case WHILE:
      eval_while(syntax, vm, program);
      return NULL;
    case COLON:
    case COLON_HAT:
    case DOT_DOT:
      return eval_string_operation_expression(syntax, vm, program);
    case BANG:
      return eval_unary_expression(syntax, vm, program);
    case EQUAL_EQUAL:
    case BANG_EQUAL:
    case GREATER_EQUAL:
    case LESS_EQUAL:
    case LESS:
    case GREATER:
      return eval_comparison_expression(syntax, vm, program);
    case EQUAL:
    case PLUS_EQUAL:
    case MINUS_EQUAL:
    case STAR_EQUAL:
    case SLASH_EQUAL:
      eval_assignment_expression(syntax, vm, program);
      return NULL;
    case CONST:
    case VAR:
      eval_variable_declarations(syntax, vm, program);
      return NULL;
    case IDENTIFIER:
      return eval_identifier(syntax, vm, program);
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
      return eval_binary_expression(syntax, vm, program);
    default:
      err_interpreter_error(syntax->token);
    break;
  }
  return NULL;
}
