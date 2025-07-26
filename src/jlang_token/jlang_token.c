#include <string.h>
#include "../jlang_source_code.h"
#include "jlang_token.h"

jl_token_t *jl_token_new(jl_source_code_t *src, jl_token_type_t type, char *literal)
{
  jl_token_t *token = malloc(sizeof(jl_token_t));
  if(token == NULL)
  {
    return NULL;
  }
  token->type = type;
  if(src != NULL)
  {
    token->literal = literal;
    token->line = src->line;
    token->column = src->column;
  }
  return token;
}
