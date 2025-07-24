#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "jlang_token.h"
#include "jlang_syntax.h"
#include "jlang_object.h"

jl_syntax_t *parse_expression(jl_token_list_t *tokens);
jl_syntax_t *parse_statement(jl_token_list_t *tokens);

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

jl_syntax_t *parse_array_declaration(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = new_syntax();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_t *token = jl_token_list_advance(tokens);
  jl_syntax_t *prev_syntax = declaration;
  do
  {
    if(token->type == COMMA)
    {
      token = jl_token_list_advance(tokens);
    }
    if(!(token->type == IDENTIFIER || token->type == NUMBER || token->type == STRING))
    {
      printf("expected IDENTIFIER, NUMBER or STRING.");
      return NULL;
    }
    jl_syntax_t *syntax = new_syntax();
    syntax->token = token;
    token = jl_token_list_peek(tokens, 0);
    prev_syntax->left = syntax;
    prev_syntax = syntax;
    token = jl_token_list_advance(tokens);
  } while(token->type == COMMA);
  if(token->type != RIGHT_BRACKET)
  {
    printf("bracket not closed");
    return NULL;
  }
  jl_token_list_advance(tokens);
  return declaration;
}

jl_syntax_t *parse_identifier(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = new_syntax();
  syntax->token = jl_token_list_peek(tokens, 0);
  if(syntax->token->type != IDENTIFIER)
  {
    return NULL;
  }
  jl_token_list_advance(tokens);
  if(jl_token_list_peek(tokens, 0)->type != LEFT_BRACKET)
  {
    syntax->right = parse_array_declaration(tokens);
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
    case LEFT_BRACKET:
      free(syntax);
      return parse_array_declaration(tokens);
    case IDENTIFIER: 
      return parse_identifier(tokens);
    case STRING:
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
    jl_syntax_t *op = new_syntax();
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
    jl_syntax_t *op = new_syntax();
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
    jl_syntax_t *op = new_syntax();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      printf("end of tokens?\n");
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
  if(token != NULL && token->type == EQUAL)
  {
    jl_syntax_t *op = new_syntax();
    op->token = jl_token_list_advance(tokens);
    if(op->token == NULL)
    {
      free(op);
      printf("end of tokens?\n");
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

jl_syntax_t *parse_variable_declarations(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = new_syntax();
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
      printf("expected identifer in variable declaration\n");
      return NULL;
    }
    jl_syntax_t *syntax = new_syntax();
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
        printf("consts need to have values assigned\n");
        return NULL;
      }
      syntax->value = new_syntax();
      syntax->value->token = jl_token_new(NULL, NIL);
    }
    token = jl_token_list_peek(tokens, 0);
    prev_syntax->left = syntax;
    prev_syntax = syntax;
  } while(token->type == COMMA);
  return declaration;
}

jl_program_t *parse_branch(jl_token_list_t *tokens)
{
  jl_program_t *program = jl_new_program();
  if(jl_token_list_peek(tokens, 0)->type != LEFT_BRACE)
  {
    return NULL;
  }
  jl_token_list_advance(tokens);
  while(jl_token_list_peek(tokens, 0)->type != RIGHT_BRACE)
  {
    if(jl_token_list_peek(tokens, 0)->type == END_OF_FILE)
    {
      printf("unexpected end of file instead of '}'");
      return NULL;
    }
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
      if(syntax == NULL)
      {
        continue;
      }
      syntax->left = prev_syntax;
      prev_syntax = syntax;
    }
    jl_token_list_advance(tokens);
    jl_program_add(program, syntax);
  }
  jl_token_list_advance(tokens);  
  return program;
}

jl_syntax_t  *parse_if(jl_token_list_t *tokens)
{
  jl_syntax_t *parent = new_syntax();
  parent->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  parent->value = parse_expression(tokens);
  if(jl_token_list_peek(tokens, 0)->type != LEFT_BRACE)
  {
    printf("Expected '{' after if statement\n");
    return NULL;
  }
  parent->branch = parse_branch(tokens);
  jl_syntax_t *syntax = parent->right;
  while(jl_token_list_peek(tokens, 0)->type == ELSE_IF)
  {
    if(parent->right == NULL)
    {
      syntax = new_syntax();
      parent->right = syntax;
    }
    else 
    {
      syntax->right = new_syntax();
      syntax = syntax->right;
    }
    syntax->token = jl_token_list_peek(tokens, 0);
    jl_token_list_advance(tokens);
    syntax->value = parse_expression(tokens);
    syntax->branch = (jl_program_t *)parse_branch(tokens);
  }
  printf("%i\n", jl_token_list_peek(tokens, 0)->type);
  if(jl_token_list_peek(tokens, 0)->type == ELSE)
  {
    syntax = new_syntax();
    syntax->token = jl_token_list_peek(tokens, 0);
    jl_token_list_advance(tokens);
    printf("%i\n", jl_token_list_peek(tokens, 0)->type);
    syntax->branch = parse_branch(tokens);
    printf("%i\n", jl_token_list_peek(tokens, 0)->type);
  }
  return parent; 
}

jl_syntax_t *parse_statement(jl_token_list_t *tokens)
{
  switch(jl_token_list_peek(tokens, 0)->type)
  {
    case VAR:
    case CONST:
      return parse_variable_declarations(tokens);
    case IF:
      return parse_if(tokens);
    default:
      return parse_expression(tokens);
  }
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
      if(syntax == NULL)
      {
        continue;
      }
      syntax->left = prev_syntax;
      prev_syntax = syntax;
    }
    jl_token_list_advance(tokens);
    jl_program_add(program, syntax);
  }
  return program;
}
