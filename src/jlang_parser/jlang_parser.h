#include "../jlang_token/jlang_token_list.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_program.h"
 
jl_program_t *parse(jl_token_list_t *tokens, jl_program_t *parent);
jl_program_t *parse_branch(jl_token_list_t *tokens, jl_program_t *parent);
jl_syntax_t *parse_line(jl_token_list_t *tokens, jl_program_t *parent);

