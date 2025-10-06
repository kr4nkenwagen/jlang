#include <stdio.h>
#include <string.h>
#include "jlang_parser.h"
#include "jlang_parse_variables.h"
#include "jlang_parse_function.h"
#include "jlang_parse_expressions.h"
#include "../jlang_token/jlang_token.h"
#include "../jlang_token/jlang_token_list.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object.h"
#include "../jlang_error.h"

jl_syntax_t *parse_variable_declarations(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = jl_syntax_new();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_t *token = jl_token_list_advance(tokens);
  jl_syntax_t *prev_syntax = declaration;
  do
  {
    if(token->type == COMMA)
    {
      token = jl_token_list_advance(tokens);
    }
    if(token->type != IDENTIFIER)
    {
      err_unexpected_syntax(token);
      return NULL;
    }
    jl_syntax_t *syntax = jl_syntax_new();
    syntax->token = token;
    if(jl_token_list_advance(tokens)->type == EQUAL)
    {
      jl_token_list_advance(tokens);
      syntax->value = parse_expression(tokens);
    }
    else 
    {
      if(declaration->token->type == CONST)
      {
        err_unassigned_const(token);
        return NULL;
      }
      syntax->value = jl_syntax_new();
      syntax->value->token = jl_token_new(NULL, NULL_OBJECT, "null");
    }
    token = jl_token_list_peek(tokens, 0);
    prev_syntax->left = syntax;
    prev_syntax = syntax;
  } while(token->type == COMMA);
  return declaration;
}

jl_syntax_t *parse_variable_remove(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = jl_syntax_new();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_t *token = jl_token_list_advance(tokens);
  jl_syntax_t *prev_syntax = declaration;
  do
  {
    if(token->type == COMMA)
    {
      token = jl_token_list_advance(tokens);
    }
    if(token->type != IDENTIFIER)
    {
      err_unexpected_syntax(token);
      return NULL;
    }
    jl_syntax_t *syntax = jl_syntax_new();
    syntax->token = token;
    token = jl_token_list_peek(tokens, 0);
    prev_syntax->left = syntax;
    prev_syntax = syntax;
    token = jl_token_list_advance(tokens);
  } while(token->type == COMMA);
   return declaration;
}

jl_syntax_t *parse_array_declaration(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = jl_syntax_new();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_t *token = jl_token_list_advance(tokens);
  jl_syntax_t *prev_syntax = declaration;
  do
  {
    if(token->type == COMMA)
    {
      token = jl_token_list_advance(tokens);
    }
    if(!(token->type == IDENTIFIER || token->type == NUMBER || token->type == STRING_WRAPPER))
    {
      err_expected_identifier_or_literal(token);
      return NULL;
    }
    jl_syntax_t *syntax = jl_syntax_new();
    syntax->token = token;
    token = jl_token_list_peek(tokens, 0);
    prev_syntax->left = syntax;
    prev_syntax = syntax;
    token = jl_token_list_advance(tokens);
  } while(token->type == COMMA);
  if(token->type != RIGHT_BRACKET)
  {
    err_bracket_not_closed(token);
    return NULL;
  }
  jl_token_list_advance(tokens);
  return declaration;
}

jl_syntax_t *parse_identifier(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = jl_syntax_new();
  syntax->token = jl_token_list_peek(tokens, 0);
  if(syntax->token->type != IDENTIFIER)
  {
    return NULL;
  }
  jl_token_list_advance(tokens);
  if(jl_token_list_peek(tokens, 0)->type == LEFT_BRACKET)
  {
    syntax->right = parse_array_declaration(tokens);
  }
  if(jl_token_list_peek(tokens, 0)->type == LEFT_PAREN)
  {
    syntax->left = parse_passed_function_args(tokens);
  }
  return syntax;
}


