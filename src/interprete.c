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

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, stack_t *vm)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  switch(syntax->token->type)
  {
    case IDENTIFIER:
      jl_object_t *obj = jl_stack_get(vm, syntax->token->literal);
      return obj;
    case STRING:
    case NUMBER:
    case NIL:
      return syntax->value;
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
