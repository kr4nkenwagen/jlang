#include <stdlib.h>

typedef enum jl_token_type{
  //single char tokens
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
  //two char tokens
  BANG_EQUAL,
  GREATER_EQUAL,
  LESS_EQUAL,
  EQUAL_EQUAL,
  //literals
  STRING_WRAPPER,
  NUMBER,
  //keywords
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
  END_OF_FILE
}jl_token_type_t;

typedef struct jl_token {
  jl_token_type_t type;
  char *lexeme;
  char *literal;
  int line;
}jl_token_t;

typedef struct jl_token_list {
  jl_token_t **list;
  size_t size;
  int count;
}jl_token_list_t;


jl_token_list_t *jl_token_list_new();
void jl_token_list_add(jl_token_list_t *list, jl_token_t *token);


jl_token_t *jl_token_new(jl_token_type_t type);
