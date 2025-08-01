#include "jlang_token/jlang_token_list.h"
#include "jlang_program.h"

void debug(jl_token_list_t *tokens, jl_program_t *program);
int count_syntax(jl_syntax_t *syntax, int num);
void count_tokens(jl_token_list_t *tokens);
void jl_print_object(jl_object_t *target);
void print_array_content(jl_object_t *arr);
