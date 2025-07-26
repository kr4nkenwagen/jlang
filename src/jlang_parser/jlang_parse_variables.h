#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_token/jlang_token_list.h"
#include "../jlang_token/jlang_token.h"

jl_syntax_t *parse_array_declaration(jl_token_list_t *tokens);
jl_syntax_t *parse_identifier(jl_token_list_t *tokens);
jl_syntax_t *parse_variable_declarations(jl_token_list_t *tokens);
