#include <string.h>
#include <stdlib.h>
#include "jlang_syntax.h"

jl_syntax_t *jl_syntax_new()
{
  jl_syntax_t * syntax = malloc(sizeof(jl_syntax_t));
  if(syntax == NULL)
  {
    return NULL;
  }
  memset(syntax, 0, sizeof(jl_syntax_t));
  return syntax;
}


