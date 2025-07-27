#include "jlang_token/jlang_token.h"

void err_unexpected_character(int line, int column, char c);
void err_eof_in_string(int line, int column);
void err_src_null();
void err_expected_identifier_or_literal(jl_token_t *token);
void err_bracket_not_closed(jl_token_t *token);
void err_end_of_tokens(jl_token_t *token);
void err_unexpected_syntax(jl_token_t *token);
void err_unassigned_const(jl_token_t *token);
void err_unexpected_eof(jl_token_t *token);
void err_illegal_operation(jl_token_t *token);
void err_redeclaration(jl_token_t *token);
void err_expected_boolean_condition(jl_token_t *token);
void err_expected_array_index(jl_token_t *token);
void err_interpreter_error(jl_token_t *token);
void err_divide_by_zero(jl_token_t *token);
void err_identifier_does_not_exist(jl_token_t *token);
void err_paren_not_closed(jl_token_t *token);
void err_incorrect_number_of_references(jl_token_t *token);
void err_modify_constant(jl_token_t *token);
