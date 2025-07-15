#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scan.h"
#include "jlang_object.h"
#include "jlang_token.h"
#include "jlang_source_code.h"
#include "jlang_error.h"

void consume_comment(jl_source_code_t *src)
{
  if(src == NULL || jl_source_code_peek(src, 0)!= '#')
  {
    return;
  }
  while(!src->is_at_end == 0)
  {
    char c = jl_source_code_advance(src);
    if(c == '\n' || c == '#')
    {
      return;
    }
  }
}

char *consume_string(jl_source_code_t *src)
{
  if(src == NULL || (jl_source_code_peek(src, 0)!= '\'' && jl_source_code_peek(src, 0) != '"'))
  {
    return NULL;
  }
  char exit_char = jl_source_code_peek(src, 0) == '"' ? '"' : '\'';
  size_t size = 1;
  while(jl_source_code_peek(src, size++) != exit_char)
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
    jl_source_code_advance(src);
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

char *consume_number(jl_source_code_t *src)
{
  if(src == NULL)
  {
    return NULL;
  }
  int is_float = 0;
  size_t size = 1;
  char character = jl_source_code_peek(src, 0);
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
    character = jl_source_code_peek(src, size++);
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
    jl_source_code_advance(src);
  }
  return number;
}

char *consume_variable_name(jl_source_code_t *src)
{
  if(src == NULL)
  {
    return NULL;
  }
  char character = jl_source_code_peek(src, 0);
  size_t size = 1;
  while(isalpha(character) || character == '_')
  {
    character = jl_source_code_peek(src, size++);
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
    jl_source_code_advance(src);
  }
  return variable;
}

int is_next_word_match(jl_source_code_t *src, char *word)
{
  if (src == NULL)
  {
    return 0;
  }
  size_t size = strlen(word);
  for(int i = 0; i < size; i++)
  {
    if(tolower(jl_source_code_peek(src, i)) != tolower(word[i]))
    {
      return 0;
    }
  }
  char after = jl_source_code_peek(src, size);
  if(after == '\n' || after == ' ' || after == '\t')
  {
    return 1;
  }
  return 0;
}

void consume_word(jl_source_code_t * src)
{
  char x;
  do
  {
    x = jl_source_code_advance(src);
  }while(x != '\0' && x != '\n' && x != ' ' );
}

jl_token_t *consume_reserved_word(jl_source_code_t *src)
{
  if(isalpha(jl_source_code_peek(src, -1)))
  {
    return NULL;
  }
  char character = jl_source_code_peek(src, 0);
  switch(character)
  {
    case 'a':
    case 'A':
    if(is_next_word_match(src, "and"))
    {
      consume_word(src);
      return jl_token_new(AND);
    }
  case 'c':
  case 'C':
    if(is_next_word_match(src, "class"))
    {
      consume_word(src);
      return jl_token_new(CLASS);
    }
  case 'e':
  case 'E':
    if(is_next_word_match(src, "else"))
    {
      consume_word(src);
      return jl_token_new(ELSE);
    }
  case 'f':
  case 'F':
    if(is_next_word_match(src, "for"))
    {
      consume_word(src);
      return jl_token_new(FOR);
    }
    else if(is_next_word_match(src, "false"))
    {
      consume_word(src);
      return jl_token_new(FALSE);
    }
    else if(is_next_word_match(src, "function"))
    {
      consume_word(src);
      return jl_token_new(FUNCTION);
    }
  case 'i':
  case 'I':
    if(is_next_word_match(src, "if"))
    {
      consume_word(src);
      return jl_token_new(IF);
    }
  case 'n':
  case 'N':
    if(is_next_word_match(src, "null"))
    {
      consume_word(src);
      return jl_token_new(NIL);
    }
  case 'o':
  case 'O':
    if(is_next_word_match(src, "or"))
    {
      consume_word(src);
      return jl_token_new(OR);
    }
  case 'p':
  case 'P':
    if(is_next_word_match(src, "print"))
    {
      consume_word(src);
      return jl_token_new(PRINT);
    }
  case 'r':
  case 'R':
    if(is_next_word_match(src, "return"))
    {
      consume_word(src);
      return jl_token_new(RETURN);
    }
  case 's':
  case 'S':
    if(is_next_word_match(src, "super"))
    {
      consume_word(src);
      return jl_token_new(SUPER);
    }
  case 't':
  case 'T':
    if(is_next_word_match(src, "this"))
    {
      consume_word(src);
      return jl_token_new(THIS);
    }
    else if(is_next_word_match(src, "true"))
    {
      consume_word(src);
      return jl_token_new(TRUE);
    }
  case 'v':
  case 'V':
    if(is_next_word_match(src, "var"))
    {
      jl_token_t *token = jl_token_new(VAR);
      consume_word(src);
      token->lexeme = consume_variable_name(src);
      return token;
    }
  case 'w':
  case 'W':
    if(is_next_word_match(src, "while"))
    {
      consume_word(src);
      return jl_token_new(WHILE);
    }
  }
  return NULL;
}

jl_token_list_t *scan(jl_source_code_t *src)
{
  if(src == NULL)
  {
    printf("src is null\n");
    return NULL;
  }
  jl_token_list_t *token_list = jl_token_list_new();
  while(src->is_at_end == 0)
  {
    switch(jl_source_code_advance(src))
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
        if(is_number(jl_source_code_peek(src, 1)))
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
       if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(BANG_EQUAL));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(BANG)); 
        }
      break;
      case '=':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(EQUAL_EQUAL));
          jl_source_code_advance(src); 
        }
        else {
          jl_token_list_add(token_list, jl_token_new(EQUAL));
        }
      break;
      case '>':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(GREATER_EQUAL));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(GREATER));  
        }
      break;
      case '<':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(LESS_EQUAL));
          jl_source_code_advance(src);
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
      case ';':
        jl_token_list_add(token_list, jl_token_new(TERMINATOR));
        break;
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
  if(token_list->count > 0 && jl_token_list_peek(token_list, 0)->type != TERMINATOR)
  {
    jl_token_list_add(token_list, jl_token_new(TERMINATOR));
  }
  jl_token_list_add(token_list, jl_token_new(END_OF_FILE));
  return token_list;
}
