
jl_syntax_t *parse_primary_expression(jl_token_list_t *tokens);
jl_syntax_t *parse_string_operations(jl_token_list_t * tokens);
jl_syntax_t *parse_statement(jl_token_list_t *tokens);
jl_syntax_t *parse_unary(jl_token_list_t * tokens);
jl_syntax_t *parse_multiplicitive(jl_token_list_t *tokens);
jl_syntax_t *parse_additive(jl_token_list_t * tokens);
jl_syntax_t *parse_comparision(jl_token_list_t * tokens);
jl_syntax_t *parse_equality(jl_token_list_t * tokens);
jl_syntax_t *parse_assignment(jl_token_list_t * tokens);
jl_syntax_t *parse_expression(jl_token_list_t *tokens);
