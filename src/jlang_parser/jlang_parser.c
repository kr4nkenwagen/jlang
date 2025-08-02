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

jl_program_t *parse_branch(jl_token_list_t *tokens)
{
  jl_program_t *program = jl_new_program();
  if(jl_token_list_peek(tokens, 0)->type == TERMINATOR)
  {
    jl_token_list_advance(tokens);
  }
  if(jl_token_list_peek(tokens, 0)->type != LEFT_BRACE)
  {
    err_bracket_not_opened(jl_token_list_peek(tokens, 0));
    return NULL;
  }
  jl_token_list_advance(tokens);
  while(jl_token_list_peek(tokens, 0)->type != RIGHT_BRACE)
  {
    if(jl_token_list_peek(tokens, 0)->type == END_OF_FILE)
    {
      err_unexpected_eof(jl_token_list_peek(tokens, 0));
      return NULL;
    }
    jl_syntax_t *syntax = NULL;
    jl_syntax_t *prev_syntax = NULL;
    while(jl_token_list_peek(tokens, 0)->type != TERMINATOR &&
          jl_token_list_peek(tokens, 0)->type != RIGHT_BRACE)
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

jl_syntax_t *parse_line(jl_token_list_t *tokens)
{
  jl_syntax_t *syntax = NULL;
  jl_syntax_t *prev_syntax = NULL;
  while(jl_token_list_peek(tokens, 0)->type != TERMINATOR &&
        jl_token_list_peek(tokens, 0)->type != RIGHT_PAREN)
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
  while(jl_token_list_peek(tokens, 0)->type == TERMINATOR)
  {
    jl_token_list_advance(tokens);
  }
  return syntax;
}

jl_program_t *parse(jl_token_list_t *tokens)
{
  jl_program_t *program = jl_new_program(); 
  while(jl_token_list_peek(tokens, 0)->type != END_OF_FILE)
  {
    jl_syntax_t *syntax = NULL;
    jl_syntax_t *prev_syntax = NULL;
    while(jl_token_list_peek(tokens, 0)->type != TERMINATOR && 
          jl_token_list_peek(tokens, 0)->type != RIGHT_BRACE &&
          jl_token_list_peek(tokens, 0)->type != LEFT_BRACE)
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
