#include <string.h>
#include "jlang_object.h"
#include "jlang_syntax.h"
#include "jlang_token.h"
#include "jlang_program.h"
#include "jlang_common.h"
#include "vm.h"
#include "stack.h"


jl_object_t *eval_primary_expression(jl_syntax_t *syntax, stack_t *vm);

jl_object_t *interprete(jl_program_t *program, stack_t *vm)
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

jl_object_t *parse_number(jl_syntax_t *syntax)
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

jl_object_t *eval_comparison_expression(jl_syntax_t *syntax, stack_t *vm)
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

jl_object_t *eval_binary_expression(jl_syntax_t *syntax, stack_t *vm)
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
  else if(syntax->token->type == SLASH)
  {
    return jl_divide(left_hand_side, right_hand_side);
  }
   else if(syntax->token->type == MODULUS)
  {
    return jl_modulus(left_hand_side, right_hand_side);
  }
  return NULL;
}

void eval_assignment_expression(jl_syntax_t *syntax, stack_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  jl_object_t *left_hand_side = eval_primary_expression(syntax->left, vm);
  jl_object_t *right_hand_side = eval_primary_expression(syntax->right, vm);
  jl_assign(left_hand_side, right_hand_side);
}

void eval_variable_declarations(jl_syntax_t *syntax, stack_t *vm)
{
  if(syntax == NULL)
  {
    return;
  }
  int is_const = syntax->token->type == CONST;
  syntax = syntax->left;
  while(syntax != NULL)
  {
    if(jl_stack_get(vm, syntax->token->literal) != NULL)
    {
      printf("Variable '%s' is already declared", syntax->token->literal);
      return;
    }
    jl_object_t *obj = eval_primary_expression(syntax->value, vm);
    obj->name = syntax->token->literal;
    stack_push(vm, obj);
    syntax = syntax->left; 
  }
  
}

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, stack_t *vm)
{

  if(syntax == NULL)
  {
    return NULL;
  }
  switch(syntax->token->type)
  {
    case EQUAL_EQUAL:
    case BANG_EQUAL:
    case GREATER_EQUAL:
    case LESS_EQUAL:
    case LESS:
    case GREATER:
      return eval_comparison_expression(syntax, vm);
    case EQUAL:
      eval_assignment_expression(syntax, vm);
      return NULL;
    case CONST:
    case VAR:
      eval_variable_declarations(syntax, vm);
      return NULL;
    case IDENTIFIER:
      jl_object_t *obj = jl_stack_get(vm, syntax->token->literal);
      return obj;
    case STRING:
      return jl_new_string(syntax->token->literal);
    case NUMBER:
      return parse_number(syntax);
    case NIL:
      return jl_new_null();
    case PLUS:
    case MINUS:
    case STAR:
    case SLASH:
    case MODULUS: 
      return eval_binary_expression(syntax, vm);
    default:
      printf("interpreter error: %i\n", syntax->token->type);
    break;
  }
  return NULL;
}
