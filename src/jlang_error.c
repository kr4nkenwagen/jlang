#include <stdio.h>
#include "jlang_error.h"
#include "jlang_token/jlang_token.h"

void err_unexpected_character(int line, int column, char c)
{
  fprintf(stderr, "[ERROR:%i:%i] Unexpected character '%s'\n", line, column, c);
}

void err_eof_in_string(int line, int column)
{
  fprintf(stderr, "[ERROR:%i:%i] String did not terminate\n", line, column);
}

void err_src_null()
{
  fprintf(stderr, "[ERROR] File was not found \n");
}

void err_expected_identifier_or_literal(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Expected identifier or literal. Got '%s'\n", token->line, token->column, token->literal);
}

void err_bracket_not_closed(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Bracket not closed\n", token->line, token->column);
}

void err_bracket_not_opened(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Bracket not opened\n", token->line, token->column);
}

void err_end_of_tokens(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Unexpected end of tokens\n", token->line, token->column);
}

void err_unexpected_syntax(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Unexpected token: %s\n", token->line, token->column, token->literal);
}

void err_unassigned_const(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Unassign constant\n", token->line, token->column);
}

void err_unexpected_eof(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Unexpected EOF\n", token->line, token->column);
}

void err_illegal_operation(jl_token_t *token)
{
  fprintf(stderr, "%s\n", token->literal);
  fprintf(stderr, "[ERROR:%i:%i] Illegal operation\n", token->line, token->column);
}

void err_redeclaration(jl_token_t *token)
{
 fprintf(stderr, "[ERROR:%i:%i] Variable '%s' is already declared\n", token->line, token->column, token->literal);
}

void err_expected_boolean_condition(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Expected a boolean as condition\n", token->line, token->column);
}

void err_expected_array_index(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Expected array index\n", token->line, token->column);
}

void err_interpreter_error(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Interpreter error: %s\n", token->line, token->column, token->literal);
}

void err_divide_by_zero(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] Divide by zero\n", token->line, token->column);
}

void err_identifier_does_not_exist(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i:%i] '%s' is not defined\n", token->line, token->column, token->literal);
}

void err_paren_not_closed(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Paren not closed\n", token->line, token->column);
}

void err_incorrect_number_of_references(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Incorrect number of references\n", token->line, token->column);
}

void err_modify_constant(jl_token_t *token)
{
  fprintf(stderr, "[ERROR:%i%i] Tried to change state of a constant\n", token->line, token->column);
}
