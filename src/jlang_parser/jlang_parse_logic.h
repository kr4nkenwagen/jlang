#include "../jlang_token/jlang_token_list.h"
#include "../jlang_program.h"
 
jl_syntax_t *parse_while(jl_token_list_t *tokens);
jl_syntax_t  *parse_if(jl_token_list_t *tokens);
jl_syntax_t *parse_variable_declarations(jl_token_list_t *tokens);
