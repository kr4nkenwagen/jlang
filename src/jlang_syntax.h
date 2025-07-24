#include "jlang_token.h"
#include "jlang_object.h"

typedef struct jl_syntax jl_syntax_t;

typedef struct jl_syntax {
  jl_token_t *token;
  jl_syntax_t *left;
  jl_syntax_t *right;
  jl_syntax_t *value; 
  void *branch; 
} jl_syntax_t;
