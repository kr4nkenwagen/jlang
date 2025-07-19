#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "jlang_token.h"
#include "jlang_syntax.h"
#include "jlang_object.h"

jl_syntax_t *parse_expression(jl_token_list_t *tokens);

jl_syntax_t *new_syntax()
{
  jl_syntax_t * syntax = malloc(sizeof(jl_syntax_t));
  if(syntax == NULL)
  {
    printf("error creating syntax node\n");
    return NULL;
  }
  memset(syntax, 0, sizeof(jl_syntax_t));
  return syntax;
}

jl_syntax_t *parse_string(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = new_syntax(); 
  if(syntax == NULL)
  {
    return NULL;
  }
  syntax->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  if(syntax->token == NULL)
  {
    free(syntax);
    printf("end of tokens?\n");
    return NULL;
  }
  syntax->value = jl_new_string(syntax->token->literal);
  return syntax;
}

jl_syntax_t *parse_number(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = new_syntax();
  if(syntax == NULL)
  {
    return NULL;
  }
  syntax->token = jl_token_list_peek(tokens, 0);
  if(syntax->token == NULL)
  {
    free(syntax);
    printf("end of tokens?\n");
    return NULL;
  }
  jl_token_list_advance(tokens);
  if(strchr(syntax->token->literal, '.') != NULL)
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
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token == NULL || token->type == END_OF_FILE)
  {
    return NULL;
  } 
  jl_syntax_t *syntax = new_syntax();
  switch(token->type)
  {
    case IDENTIFIER: 
      syntax->token = token;
      jl_token_list_advance(tokens);
      return syntax;
    case STRING:
      free(syntax);
      return parse_string(tokens);
    case NUMBER:
      free(syntax);
      return parse_number(tokens);
    case FALSE:
    case TRUE:
      syntax->token = token;
      jl_token_list_advance(tokens);
      return syntax;
    case NIL:
      syntax->token = token;
      syntax->value = jl_new_null();
      jl_token_list_advance(tokens);
      return syntax;
    case LEFT_PAREN:
      if(jl_token_list_advance(tokens) == NULL)
      {
        printf("End of tokens?\n");
        return NULL;
      }
      syntax = parse_expression(tokens);
      jl_token_t *token = jl_token_list_peek(tokens, 0);
      if(token == NULL || token->type != RIGHT_PAREN)
      {
        printf("ERROR: Unexpected token. expected ')'\n");
      }
      jl_token_list_advance(tokens);
      return syntax;
    case TERMINATOR:
      free(syntax);
      return NULL;
  }
  free(syntax);
  printf("syntax error: %i\n", token->type);
  return NULL;
}

jl_syntax_t *parse_unary(jl_token_list_t * tokens)
{
  jl_syntax_t *left = parse_primary_expression(tokens);
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  if(token != NULL && token->type == BANG)
  {
    jl_syntax_t *op = new_syntax();
    if(op == NULL)
    {
      return NULL;
    }
    jl_token_list_advance(tokens);
    op->token = token;
    op->left = left;
    op->right = parse_primary_expression(tokens);
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
    jl_syntax_t *op = new_syntax(); 
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
    jl_syntax_t *op = new_syntax();
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
    jl_syntax_t *op = new_syntax();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      printf("end of tokens?\n");
      return NULL;
    }
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
    jl_syntax_t *op = new_syntax();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      printf("end of tokens?\n");
      return NULL;
    }
    op->left = left;
    op->right = parse_comparision(tokens);
    left = op;
  }
  return left; 
}

jl_syntax_t *parse_expression(jl_token_list_t *tokens)
{
  return parse_equality(tokens);
}

jl_syntax_t *parse_statement(jl_token_list_t *tokens)
{

  return parse_expression(tokens);
}

jl_program_t *parse(jl_token_list_t *tokens)
{
  jl_program_t *program = jl_new_program(); 
  while(jl_token_list_peek(tokens, 0)->type != END_OF_FILE)
  {
    jl_syntax_t *syntax = NULL;
    jl_syntax_t *prev_syntax = NULL;
    while(jl_token_list_peek(tokens, 0)->type != TERMINATOR)
    {
      if(syntax == NULL)
      {
        syntax = parse_statement(tokens);
        prev_syntax = syntax;
        continue;
      }
      syntax = parse_statement(tokens);
      syntax->left = prev_syntax;
      prev_syntax = syntax;
    }
    jl_token_list_advance(tokens);
    jl_program_add(program, syntax);
  }
  return program;
}
