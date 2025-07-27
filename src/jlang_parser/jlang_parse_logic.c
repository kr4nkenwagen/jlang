#include <stdio.h>
#include <string.h>
#include "jlang_parser.h"
#include "jlang_parse_variables.h"
#include "jlang_parse_function.h"
#include "jlang_parse_expressions.h"
#include "jlang_parse_logic.h"
#include "../jlang_token/jlang_token.h"
#include "../jlang_token/jlang_token_list.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object.h"
#include "../jlang_error.h"

jl_syntax_t  *parse_if(jl_token_list_t *tokens)
{
  jl_syntax_t *parent = jl_syntax_new();
  parent->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  parent->value = parse_expression(tokens);
  while(jl_token_list_peek(tokens, 0)->type == TERMINATOR)
  {
    jl_token_list_advance(tokens);
  }
  if(jl_token_list_peek(tokens, 0)->type != LEFT_BRACE)
  {
    err_unexpected_syntax(jl_token_list_peek(tokens, 0));
    return NULL;
  }
  parent->branch = parse_branch(tokens);
  jl_syntax_t *syntax = parent;
  while(jl_token_list_peek(tokens, 0)->type == ELSE_IF)
  {
    syntax->right = jl_syntax_new();
    syntax->right->token = jl_token_list_peek(tokens, 0);
    jl_token_list_advance(tokens);
    syntax->right->value = parse_expression(tokens);
    syntax->right->branch = (jl_program_t *)parse_branch(tokens);
    syntax = syntax->right;
  }
  if(jl_token_list_peek(tokens, 0)->type == ELSE)
  {
    syntax->right = jl_syntax_new();
    syntax->right->token = jl_token_list_peek(tokens, 0);
    jl_token_list_advance(tokens);
    syntax->right->branch = parse_branch(tokens);
  }
  return parent; 
}

jl_syntax_t *parse_while(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = jl_syntax_new();
  syntax->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  syntax->value = parse_expression(tokens);
  syntax->branch = parse_branch(tokens);
  return syntax;
}


