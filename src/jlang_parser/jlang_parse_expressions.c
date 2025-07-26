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

jl_syntax_t *parse_primary_expression(jl_token_list_t *tokens)
{
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token == NULL || token->type == END_OF_FILE)
  {
    return NULL;
  } 
  jl_syntax_t *syntax = jl_syntax_new();
  switch(token->type)
  {
    case LEFT_BRACKET:
      free(syntax);
      return parse_array_declaration(tokens);
    case IDENTIFIER: 
      return parse_identifier(tokens);
    case STRING_WRAPPER:
    case NUMBER:
    case FALSE:
    case TRUE:
    case NIL:
      syntax->token = token;
      jl_token_list_advance(tokens);
      return syntax;
    case LEFT_PAREN:
      if(jl_token_list_advance(tokens) == NULL)
      {
        err_end_of_tokens(token);
        return NULL;
      }
      syntax = parse_expression(tokens);
      jl_token_t *token = jl_token_list_peek(tokens, 0);
      if(token == NULL || token->type != RIGHT_PAREN)
      {
        err_unexpected_syntax(token);
      }
      jl_token_list_advance(tokens);
      return syntax;
    case TERMINATOR:
      free(syntax);
      return NULL;
  }
  free(syntax);
  err_unexpected_syntax(token);
  return NULL;
}

jl_syntax_t *parse_statement(jl_token_list_t *tokens)
{
  switch(jl_token_list_peek(tokens, 0)->type)
  {
    case FUNCTION:
      return parse_function(tokens);
    case WHILE:
      return parse_while(tokens);
    case VAR:
    case CONST:
      return parse_variable_declarations(tokens);
    case IF:
      return parse_if(tokens);
    default:
      return parse_expression(tokens);
  }
}
jl_syntax_t *parse_string_operations(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_primary_expression(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (
     token->type == COLON || 
     token->type == COLON_HAT ||
     token->type == DOT_DOT))
  {
    jl_token_list_advance(tokens);
    jl_syntax_t *op = jl_syntax_new();
    if(op == NULL)
    {
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_primary_expression(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_unary(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_string_operations(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && token->type == BANG)
  {
    jl_token_list_advance(tokens);
    jl_syntax_t *op = jl_syntax_new();
    if(op == NULL)
    {
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_string_operations(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_multiplicitive(jl_token_list_t *tokens)
{
  jl_syntax_t *left = parse_unary(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (
        token->type == STAR || 
        token->type == SLASH || 
        token->type == MODULUS))
  {
    jl_token_list_advance(tokens);
    jl_syntax_t *op = jl_syntax_new(); 
    if(op == NULL)
    {
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_unary(tokens);
    left = op;
  }
  return left;
}

jl_syntax_t *parse_additive(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_multiplicitive(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (
        token->type == PLUS || 
        token->type == MINUS))
  {
    jl_token_list_advance(tokens);
    jl_syntax_t *op = jl_syntax_new();
    if(op == NULL)
    {
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_multiplicitive(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_comparision(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_additive(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (
        token->type == GREATER_EQUAL || 
        token->type == LESS_EQUAL || 
        token->type == GREATER || 
        token->type == LESS))
  {
    jl_syntax_t *op = jl_syntax_new();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      err_end_of_tokens(token);
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_additive(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_equality(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_comparision(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (  
        token->type == EQUAL_EQUAL || 
        token->type == BANG_EQUAL))
  {
    jl_syntax_t *op = jl_syntax_new();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      err_end_of_tokens(token);
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_comparision(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_assignment(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_equality(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && (
    token->type == EQUAL ||
    token->type == PLUS_EQUAL ||
    token->type == MINUS_EQUAL ||
    token->type == STAR_EQUAL ||
    token->type == SLASH_EQUAL
  ))
  {
    jl_syntax_t *op = jl_syntax_new();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      err_end_of_tokens(token);
      return NULL;
    }
    op->token = token;
    op->left = left;
    op->right = parse_equality(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_expression(jl_token_list_t *tokens)
{
  return parse_assignment(tokens);
}


