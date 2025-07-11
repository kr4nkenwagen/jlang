#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "jlang_token.h"
#include "jlang_syntax.h"
#include "jlang_object.h"

jl_syntax_t *parse_expression(jl_token_list_t *tokens);

int is_operator(jl_token_t * token)
{
  switch(token->type)
  {
    case MINUS:
    case PLUS:
    case STAR:
    case SLASH:
    case MODULUS:
    case GREATER:
    case LESS:
    case EQUAL_EQUAL:
    case BANG_EQUAL:
    case LESS_EQUAL:
    case GREATER_EQUAL:
      return 1;
  }
  return false;
}

jl_syntax_t *parse_string(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = malloc(sizeof(jl_syntax_t));
  if(syntax == NULL)
  {
    return NULL;
  }
  syntax->token = jl_token_list_advance(tokens);
  syntax->value = jl_new_string(syntax->token->literal);
  return syntax;
}

jl_syntax_t *parse_number(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = malloc(sizeof(jl_syntax_t));
  if(syntax == NULL)
  {
    return NULL;
  }
  syntax->token = jl_token_list_advance(tokens);
  if(strchr(syntax->token->literal, '.'))
  {
    syntax->value = jl_new_float(atof(syntax->token->literal));
  }
  else 
  {
    syntax->value = jl_new_int(atoi(syntax->token->literal));
  }
  return syntax;
}

jl_syntax_t *parse_primary_expression(jl_token_list_t *tokens)
{
  switch(jl_token_list_peek(tokens, 0)->type)
  {
    case STRING:
      return parse_string(tokens);
    break;
    case NUMBER:
      return parse_number(tokens);
    break;
    case LEFT_PAREN:
      jl_token_list_advance(tokens);
      jl_syntax_t *syntax = parse_expression(tokens);
      jl_token_list_advance(tokens);
      return syntax;
    break;
  }
  return NULL;
}

jl_syntax_t *parse_multiplicitive(jl_token_list_t *tokens)
{
  jl_syntax_t *left = malloc(sizeof(jl_syntax_t));
  left = parse_primary_expression(tokens);
  while(jl_token_list_peek(tokens, 0)->type == STAR || jl_token_list_peek(tokens, 0)->type == SLASH || jl_token_list_peek(tokens, 0)->type == MODULUS)
  {
    left->token = jl_token_list_advance(tokens);
    left->right = parse_primary_expression(tokens);
  }
  return left;
}

jl_syntax_t *parse_additive(jl_token_list_t * tokens)
{
  jl_syntax_t *left = malloc(sizeof(jl_syntax_t));
  left = parse_multiplicitive(tokens);
  while(jl_token_list_peek(tokens, 0)->type == PLUS || jl_token_list_peek(tokens, 0)->type == MINUS)
  {
    left->token = jl_token_list_advance(tokens);
    left->right = parse_multiplicitive(tokens);
  }
  return left; 
}

jl_syntax_t *parse_expression(jl_token_list_t *tokens)
{
  return parse_additive(tokens);
}

jl_syntax_t *parse_statement(jl_token_list_t *tokens)
{

  return parse_expression(tokens);
}

jl_token_list_t *parse(jl_token_list_t *tokens)
{
  while(jl_token_list_peek(tokens, 0)->type == EOF)
  {
    jl_syntax_t *syntax = malloc(sizeof(jl_syntax_t));
    if(is_operator(jl_token_list_peek(tokens, 0)))
    {
    }
  }
  return NULL;
}
