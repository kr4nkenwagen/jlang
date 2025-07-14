#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scan.h"
#include "jlang_object.h"
#include "jlang_token.h"
#include "jlang_error.h"


source_code_t *new_source_code()
{
  source_code_t *src = malloc(sizeof(source_code_t));
  if(src == NULL)
  {
    return NULL;
  }
  src->is_at_end = 0;
  src->pointer = -1;
  src->line = 0;
  src->column = 0;
  return src;
}

source_code_t *open_src(char *src)
{
  FILE *file_ptr = fopen(src, "r");
  if(file_ptr == NULL)
  {
    return NULL;
  }
  source_code_t *src_code = new_source_code(); 
  if(src_code == NULL)
  {
    return NULL;
  }
  fseek(file_ptr, 0L, SEEK_END);
  src_code->size = ftell(file_ptr);
  fseek(file_ptr, 0L, SEEK_SET);
  src_code->src = calloc(sizeof(char), src_code->size);
  char *src_line= calloc(sizeof(char), src_code->size);
  if(src_line == NULL)
  {
    free(file_ptr);
    return NULL;
  }
  while(fgets(src_line, src_code->size, file_ptr))
  {
    strcat(src_code->src, src_line);     
  }
  free(src_line);
  return src_code;
}

source_code_t *from_repl_line(char *line)
{
  if(line == NULL)
  {
    return NULL;
  }
  source_code_t *src = new_source_code();
  {
    return NULL;
  }
  src->size = strlen(line); 
  return src;
}

char advance(source_code_t *src)
{
  if(src == NULL || src->is_at_end == 1)
  {
    return 0; 
  }
  if(src->pointer++ == src->size)
  {
    src->is_at_end = 1;
  }
  src->column++;
  if(src->src[src->pointer] == '\n')
  {
    src->line++;
    src->column = 0;
  }
  return src->src[src->pointer];
}

char peek(source_code_t *src, int distance)
{
  if(src == NULL || src->pointer + distance >= src->size)
  {
    return 0;
  }
  return src->src[src->pointer + distance];
}

void consume_comment(source_code_t *src)
{
  if(src == NULL || peek(src, 0)!= '#')
  {
    return;
  }
  while(!src->is_at_end == 0)
  {
    char c = advance(src);
    if(c == '\n' || c == '#')
    {
      return;
    }
  }
}

char *consume_string(source_code_t *src)
{
  if(src == NULL || (peek(src, 0)!= '\'' && peek(src, 0) != '"'))
  {
    return NULL;
  }
  char exit_char = peek(src, 0) == '"' ? '"' : '\'';
  size_t size = 1;
  while(peek(src, size++) != exit_char)
  {
    if(src->pointer + size >= src->size)
    {
      err_eof_in_string(src->line, src->column);
      return NULL;
    }
  }
  char *result = malloc(sizeof(char) * size - 1);
  if(result == NULL)
  {
    return NULL;
  }
  strncpy(result, src->src + src->pointer + 1, size - 2);
  result[size] = '\0';
  for(int i = 0; i < size; i++)
  {
    advance(src);
  }
  return result;
}

int is_number(char character)
{
  switch (character) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      return 1;
    break;
    default:
      return 0;
    break;
  }
}

char *consume_number(source_code_t *src)
{
  if(src == NULL)
  {
    return NULL;
  }
  int is_float = 0;
  size_t size = 1;
  char character = peek(src, 0);
  while(is_number(character))
  {
    if(character == '.')
    {
      if(is_float)
      {
        err_unexpected_character(src->line, src->column, character);
        return NULL;
      }
      is_float = 1;
    }
    character = peek(src, size++);
  }
  size--;
  char *number = malloc(sizeof(char) * size);
  if(number == NULL)
  {
    return NULL;
  }
  strncpy(number, src->src + src->pointer, size);
  for(int i = 0; i < size; i++)
  {
    advance(src);
  }
  return number;
}

char *consume_variable_name(source_code_t *src)
{
  if(src == NULL)
  {
    return NULL;
  }
  char character = peek(src, 0);
  size_t size = 1;
  while(isalpha(character) || character == '_')
  {
    character = peek(src, size++);
  }
  size--;
  char *variable = malloc(sizeof(char) * size);
  if(variable == NULL)
  {
    return NULL;
  }
  strncpy(variable, src->src + src->pointer, size);
  for(int i = 0; i < size; i++)
  {
    advance(src);
  }
  return variable;
}

int is_next_word_match(source_code_t *src, char *word)
{
  if (src == NULL)
  {
    return 0;
  }
  size_t size = strlen(word);
  for(int i = 0; i < size; i++)
  {
    if(tolower(peek(src, i)) != tolower(word[i]))
    {
      return 0;
    }
  }
  char after = peek(src, size);
  if(after == '\n' || after == ' ' || after == '\t')
  {
    return 1;
  }
  return 0;
}

jl_token_t *consume_reserved_word(source_code_t *src)
{
  if(isalpha(peek(src, -1)))
  {
    return NULL;
  }
  char character = peek(src, 0);
  switch(character)
  {
    case 'a':
    case 'A':
    if(is_next_word_match(src, "and"))
    {
      advance(src);
      advance(src);
      return jl_token_new(AND);
    }
  case 'c':
  case 'C':
    if(is_next_word_match(src, "class"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(CLASS);
    }
  case 'e':
  case 'E':
    if(is_next_word_match(src, "else"))
    {
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(ELSE);
    }
  case 'f':
  case 'F':
    if(is_next_word_match(src, "for"))
    {
      advance(src);
      advance(src);
      return jl_token_new(FOR);
    }
    else if(is_next_word_match(src, "false"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(FALSE);
    }
    else if(is_next_word_match(src, "function"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(FUNCTION);
    }
  case 'i':
  case 'I':
    if(is_next_word_match(src, "if"))
    {
      advance(src);
      return jl_token_new(IF);
    }
  case 'n':
  case 'N':
    if(is_next_word_match(src, "null"))
    {
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(NIL);
    }
  case 'o':
  case 'O':
    if(is_next_word_match(src, "or"))
    {
      advance(src);
      return jl_token_new(OR);
    }
  case 'p':
  case 'P':
    if(is_next_word_match(src, "print"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(PRINT);
    }
  case 'r':
  case 'R':
    if(is_next_word_match(src, "return"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(RETURN);
    }
  case 's':
  case 'S':
    if(is_next_word_match(src, "super"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(SUPER);
    }
  case 't':
  case 'T':
    if(is_next_word_match(src, "this"))
    {
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(THIS);
    }
    else if(is_next_word_match(src, "true"))
    {
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(TRUE);
    }
  case 'v':
  case 'V':
    if(is_next_word_match(src, "var"))
    {
      jl_token_t *token = jl_token_new(VAR);
      advance(src);
      advance(src);
      token->lexeme = consume_variable_name(src);
      return token;
    }
  case 'w':
  case 'W':
    if(is_next_word_match(src, "while"))
    {
      advance(src);
      advance(src);
      advance(src);
      advance(src);
      return jl_token_new(WHILE);
    }
  }
  return NULL;
}

jl_token_list_t *scan(source_code_t *src)
{
  if(src == NULL)
  {
    return NULL;
  }
  jl_token_list_t *token_list = jl_token_list_new();
  while(!src->is_at_end == 0)
  {
    switch(advance(src))
    {
      case '(':
        jl_token_list_add(token_list,  jl_token_new(LEFT_PAREN));
      break;
      case ')':
        jl_token_list_add(token_list,  jl_token_new(RIGHT_PAREN));
      break;
      case '{':
        jl_token_list_add(token_list,  jl_token_new(LEFT_BRACE));
      break;
      case '}':
        jl_token_list_add(token_list,  jl_token_new(RIGHT_BRACE));
      break;
      case ',':
        jl_token_list_add(token_list,  jl_token_new(COMMA));
      break;
      case '.':
        if(is_number(peek(src, 1)))
        {
          jl_token_t *token_number = jl_token_new(NUMBER);
          token_number->literal = consume_number(src);
          jl_token_list_add(token_list, token_number);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(DOT));
        }
      break;
      case '-':
        jl_token_list_add(token_list,  jl_token_new(MINUS));
      break;
      case '+':
        jl_token_list_add(token_list,  jl_token_new(PLUS));
      break;
      case ';':
        jl_token_list_add(token_list,  jl_token_new(SEMICOLON));
     break;
      case '/':
        jl_token_list_add(token_list,  jl_token_new(SLASH));
      break;
      case '*':
        jl_token_list_add(token_list,  jl_token_new(STAR));
      break;
      case '\'':
      case '"':
        jl_token_t *token_string = jl_token_new(STRING);
        token_string->literal = consume_string(src);
        jl_token_list_add(token_list, token_string);
      break; 
      case '!':
       if(peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(BANG_EQUAL));
          advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(BANG)); 
        }
      break;
      case '=':
        if(peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(EQUAL_EQUAL));
          advance(src); 
        }
        else {
          jl_token_list_add(token_list, jl_token_new(EQUAL));
        }
      break;
      case '>':
        if(peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(GREATER_EQUAL));
          advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(GREATER));  
        }
      break;
      case '<':
        if(peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(LESS_EQUAL));
          advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(LESS));
        }
      break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        jl_token_t *token_number = jl_token_new(NUMBER);
        token_number->literal = consume_number(src);
        jl_token_list_add(token_list, token_number);
      break;
      case '\n':
      case '\t':
      case ' ':
      break;
      case '#':
        consume_comment(src);
      break;
      default:
        if(src->src[src->pointer] == 0)
        {
          break;
        }
        jl_token_t *token = consume_reserved_word(src);
        if(token == NULL)
        {
          printf("[ERROR:%i:%i] Unexpected character '%c'\n", src->line, src->column, src->src[src->pointer]);
          break;
        }
        jl_token_list_add(token_list, token);
    }
  }
  jl_token_list_add(token_list, jl_token_new(END_OF_FILE));
  return token_list;
}
