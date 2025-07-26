#include <stdlib.h>
#include "../jlang_source_code.h"

typedef enum jl_token_type{
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  BANG,
  EQUAL,
  GREATER,
  LESS,
  COMMENT,
  MODULUS,
  TERMINATOR,
  COLON,
  BANG_EQUAL,
  GREATER_EQUAL,
  LESS_EQUAL,
  EQUAL_EQUAL,
  COLON_HAT,
  DOT_DOT,
  STRING_WRAPPER,
  NUMBER,
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUNCTION,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  END_OF_FILE,
  IDENTIFIER,
  CONST,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  ELSE_IF,
  PLUS_EQUAL,
  MINUS_EQUAL,
  STAR_EQUAL,
  SLASH_EQUAL
}jl_token_type_t;

typedef struct jl_token {
  jl_token_type_t type;
  char *literal;
  int column;
  int line;
}jl_token_t;

jl_token_t *jl_token_new(jl_source_code_t *src, jl_token_type_t type, char *literal);
