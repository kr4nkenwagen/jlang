#include "../jlang_token/jlang_token_list.h"
#include "../jlang_program.h"
 
jl_syntax_t *parse_while(jl_token_list_t *tokens, jl_program_t *parent);
jl_syntax_t *parse_if(jl_token_list_t *tokens, jl_program_t *parent);
jl_syntax_t *parse_for(jl_token_list_t *tokens, jl_program_t *parent);
jl_syntax_t *parse_return(jl_token_list_t *tokens);
jl_syntax_t *parse_break(jl_token_list_t *tokens);
jl_syntax_t *parse_continue(jl_token_list_t *tokens);
