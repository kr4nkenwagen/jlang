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
  printf("parsing number\n");
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
    case STRING:
      free(syntax);
      return parse_string(tokens);
    break;
    case NUMBER:
      free(syntax);
      return parse_number(tokens);
    break;
    case FALSE:
    case TRUE:
    case NIL:
      syntax->token = token;
      jl_token_list_advance(tokens);
      return syntax;
    break;
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
      jl_token_list_advance(tokens);
      free(syntax);
      return NULL;
    break;
  }
  free(syntax);
  printf("syntax errors: %i\n", token->type);
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
  jl_token_t *token = jl_token_list_peek(tokens, 0);
  jl_syntax_t *left = parse_unary(tokens);
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
    printf("parsing multiplication\n");
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
    printf("parsing addition\n");
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

//******************
//* DEBUG FUNCTION *
//******************
int count_syntax(jl_syntax_t *syntax, int num)
{
  printf("syntax level: %i tokens: %i\n", num, syntax->token->type); 
  if(syntax->left != NULL)
  {
    num += count_syntax(syntax->left, num);
  }
  if(syntax->right != NULL)
  {
    num += count_syntax(syntax->right, num); 
  }
  num++;
  return num;
}

jl_program_t *parse(jl_token_list_t *tokens)
{
  jl_program_t *program = jl_new_program(); 
  while(jl_token_list_peek(tokens, 0)->type != END_OF_FILE)
  {
    jl_program_add(program, parse_statement(tokens));
    printf("syntax count: %i\n", count_syntax(program->statements[program->count - 1], 0));
  }
  return program;
}
