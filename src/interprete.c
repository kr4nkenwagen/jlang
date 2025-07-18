#include "jlang_object.h"
#include "jlang_syntax.h"
#include "jlang_token.h"
#include "jlang_program.h"
#include "jlang_common.h"


jl_object_t *eval_primary_expression(jl_syntax_t *syntax);

jl_object_t *interprete(jl_program_t *program)
{
  if(program == NULL)
  {
    return NULL;
  }
  for(int i = 0; i < program->count; i++)
  {
    jl_print_object(eval_primary_expression(program->statements[i]));
  }
}

jl_object_t *eval_binary_expression(jl_syntax_t *syntax)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  if(syntax->token->type == PLUS)
  {
    return jl_add(syntax->left->value, syntax->right->value);
  }
  else if(syntax->token->type == MINUS)
  {
    return jl_subtract(syntax->left->value, syntax->right->value);
  }
  else if(syntax->token->type == STAR)
  {
    return jl_multiply(syntax->left->value, syntax->right->value);
  }
  else if(syntax->token->type == SLASH)
  {
    return jl_divide(syntax->left->value, syntax->right->value);
  }
   else if(syntax->token->type == MODULUS)
  {
    return jl_modulus(syntax->left->value, syntax->right->value);
  }
  return NULL;
}

jl_object_t *eval_primary_expression(jl_syntax_t *syntax)
{
  if(syntax == NULL)
  {
    return NULL;
  }
  switch(syntax->token->type)
  {
    case STRING:
    case NUMBER:
    case NIL:
      return syntax->value;
    case PLUS:
    case MINUS:
    case STAR:
    case SLASH:
    case MODULUS: 
      return eval_binary_expression(syntax);
    default:
      printf("interpreter error: %i\n", syntax->token->type);
    break;
  }
  return NULL;
}
