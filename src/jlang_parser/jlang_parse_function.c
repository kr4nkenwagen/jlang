#include "jlang_parser.h"
#include "jlang_parse_expressions.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_token/jlang_token_list.h"
#include "../jlang_error.h"

jl_program_t *parse_function_args(jl_token_list_t *tokens)
{
  jl_program_t *args = jl_new_program();
  if(jl_token_list_peek(tokens, 0)->type != LEFT_PAREN)
  {
    err_unexpected_syntax(jl_token_list_peek(tokens, 0));
    return NULL;
  }
  jl_token_t *token = jl_token_list_advance(tokens);
  do
  {
    if(token->type == COMMA)
    {
      token = jl_token_list_advance(tokens);
    }
    if(token->type != VAR && token->type != CONST)
    {
      break;
    }
    jl_syntax_t *declaration = jl_syntax_new();
    declaration->token = token;
    token = jl_token_list_advance(tokens);
    if(token->type != IDENTIFIER)
    {
      err_unexpected_syntax(token);
      return NULL;
    }
    jl_syntax_t *syntax = jl_syntax_new();
    declaration->left = syntax;
    syntax->token = token;
    if(jl_token_list_advance(tokens)->type == EQUAL)
    {
      jl_token_list_advance(tokens);
      syntax->value = parse_expression(tokens);
    }
    else 
    {
      syntax->value = jl_syntax_new();
      syntax->value->token = jl_token_new(NULL, NIL, "null");
    }
    token = jl_token_list_peek(tokens, 0);
    syntax->left = jl_syntax_new();
    syntax->left->token = jl_token_new(NULL, TERMINATOR, ";");
    jl_program_add(args, declaration);
  } while(token->type == COMMA);
  if(jl_token_list_peek(tokens, 0)->type != RIGHT_PAREN)
  {
    err_unexpected_syntax(jl_token_list_peek(tokens, 0));
    return NULL;
  }
  jl_token_list_advance(tokens);
  return args;
}


jl_syntax_t *parse_function(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = jl_syntax_new();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  jl_syntax_t *syntax = jl_syntax_new();
  declaration->right = syntax;
  syntax->token = jl_token_list_peek(tokens, 0);
  if(syntax->token->type != IDENTIFIER)
  {
    err_unexpected_syntax(syntax->token);
  }
  jl_token_list_advance(tokens);
  
  syntax->args = parse_function_args(tokens);
  if(jl_token_list_peek(tokens, 0)->type == TERMINATOR)
  {
    jl_token_list_advance(tokens);
  }
  if(jl_token_list_peek(tokens, 0)->type == LEFT_BRACE)
  {
    syntax->branch = parse_branch(tokens); 
  }
  else 
  {
    jl_program_t *program = jl_new_program();
    syntax->branch = program;
    jl_program_add(program, parse_line(tokens));
  }
  return declaration;
}

jl_syntax_t *parse_passed_function_args(jl_token_list_t *tokens)
{
  jl_syntax_t *declaration = jl_syntax_new();
  declaration->token = jl_token_list_peek(tokens, 0);
  jl_token_t *token = jl_token_list_advance(tokens);
  jl_syntax_t *prev_syntax = declaration;
  while(token->type != RIGHT_PAREN)
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
  }
  if(token->type != RIGHT_PAREN)
  {
    err_paren_not_closed(token);
    return NULL;
  }
  jl_token_list_advance(tokens);
  return declaration;
}


jl_syntax_t *parse_function_print(jl_token_list_t *tokens)
{
  jl_syntax_t *parent = jl_syntax_new();
  parent->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  parent->value = parse_expression(tokens);
  return parent;
}

jl_syntax_t *parse_function_print_line(jl_token_list_t *tokens)
{
  jl_syntax_t *parent = jl_syntax_new();
  parent->token = jl_token_list_peek(tokens, 0);
  jl_token_list_advance(tokens);
  parent->value = parse_expression(tokens);
  return parent;
}
