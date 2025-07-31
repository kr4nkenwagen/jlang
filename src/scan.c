#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scan.h"
#include "jlang_object/jlang_object.h"
#include "jlang_token/jlang_token.h"
#include "jlang_token/jlang_token_list.h"
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
  for(int i = 0; i < size - 1; i++)
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
      return 1;
    break;
    default:
      return 0;
    break;
  }
}

int is_end_of_word(char c)
{
  switch(c)
  {
    case '\0':
    case '\n':
    case '\t':
    case ' ':
    case ';':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case ':':
    case '=':
    case '+':
    case '-':
    case '/':
    case '*':
      return 1;
  }
  return 0;
}

char *consume_word(jl_source_code_t * src)
{
  size_t size = 32;
  int i = 0;
  char * word = malloc(sizeof(char) * size);
  for(; is_end_of_word(jl_source_code_peek(src, 1)) == 0; i++)
  {
    if(i - 1 == size)
    {
      size *=2;
      char *tmp_word = malloc(sizeof(char) * size);
      memcpy(tmp_word, word, size/2);
      free(word);
      word = tmp_word;
    }
    word[i] = jl_source_code_peek(src, 0);
    jl_source_code_advance(src);
  }
  word[i++] = jl_source_code_peek(src, 0);
  word[i] = '\0';
  return word;
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
  while(is_number(character) || character == '.')
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
  for(int i = 0; i < size - 1; i++)
  {
    jl_source_code_advance(src);
  }
  return number;
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
  if(after == '\n' || after == ' ' || after == '\t' || after == ';' || src->pointer + size <= src->size)
  {
    return 1;
  }
  return 0;
}

jl_token_t *consume_identifier(jl_source_code_t *src)
{
  if(isalpha(jl_source_code_peek(src, -1)))
  {
    return NULL;
  }
  jl_token_t *token = jl_token_new(src, IDENTIFIER, consume_word(src));
  return token;
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
      return jl_token_new(src, AND, consume_word(src));
    }
  case 'c':
  case 'C':
    if(is_next_word_match(src, "class"))
    {
      return jl_token_new(src, CLASS, consume_word(src));
    }
    else if(is_next_word_match(src, "const"))
    {
        return jl_token_new(src, CONST, consume_word(src));
      }
  case 'e':
  case 'E':
    if(is_next_word_match(src, "else"))
    {
      if(tolower(jl_source_code_peek(src, 6)) == 'i' && tolower(jl_source_code_peek(src, 7)) == 'f')
      {
        return jl_token_new(src, ELSE_IF, strcat(consume_word(src), consume_word(src)));
      }
      return jl_token_new(src, ELSE, consume_word(src));
    }
  case 'f':
  case 'F':
    if(is_next_word_match(src, "for"))
    {
      return jl_token_new(src, FOR, consume_word(src));
    }
    else if(is_next_word_match(src, "false"))
    {
      return jl_token_new(src, FALSE, consume_word(src));
    }
    else if(is_next_word_match(src, "fn"))
    {
      return jl_token_new(src, FUNCTION, consume_word(src));
    }
  case 'i':
  case 'I':
    if(is_next_word_match(src, "if"))
    {
      return jl_token_new(src, IF, consume_word(src));
    }
  case 'n':
  case 'N':
    if(is_next_word_match(src, "null"))
    {
      return jl_token_new(src, NIL, consume_word(src));
    }
  case 'o':
  case 'O':
    if(is_next_word_match(src, "or"))
    {
      return jl_token_new(src, OR, consume_word(src));
    }
  case 'p':
  case 'P':
    if(is_next_word_match(src, "println"))
    {
      return jl_token_new(src, PRINT_LINE, consume_word(src));
    }
    else if(is_next_word_match(src, "print"))
    {
      return jl_token_new(src, PRINT, consume_word(src));
    }
  case 'r':
  case 'R':
    if(is_next_word_match(src, "return"))
    {
      return jl_token_new(src, RETURN, consume_word(src));
    }
  case 's':
  case 'S':
    if(is_next_word_match(src, "super"))
    {
      return jl_token_new(src, SUPER, consume_word(src));
    }
  case 't':
  case 'T':
    if(is_next_word_match(src, "this"))
    {
      return jl_token_new(src, THIS, consume_word(src));
    }
    else if(is_next_word_match(src, "true"))
    {
      return jl_token_new(src, TRUE, consume_word(src));
    }
  case 'v':
  case 'V':
    if(is_next_word_match(src, "var"))
    {
      return jl_token_new(src, VAR, consume_word(src));
    }
  case 'w':
  case 'W':
    if(is_next_word_match(src, "while"))
    {
      return jl_token_new(src, WHILE, consume_word(src));
    }
  }
  return NULL;
}

jl_token_list_t *scan(jl_source_code_t *src)
{
  if(src == NULL)
  {
    err_src_null(); 
    return NULL;
  }
  jl_token_list_t *token_list = jl_token_list_new();
  while(src->is_at_end == 0)
  {
    switch(jl_source_code_advance(src))
    {
      case '(':
        jl_token_list_add(token_list,  jl_token_new(src, LEFT_PAREN, "("));
      break;
      case ')':
        jl_token_list_add(token_list,  jl_token_new(src, RIGHT_PAREN, ")"));
      break;
      case '{':
        jl_token_list_add(token_list,  jl_token_new(src, LEFT_BRACE, "{"));
      break;
      case '}':
        jl_token_list_add(token_list, jl_token_new(src, TERMINATOR, ";"));
        jl_token_list_add(token_list, jl_token_new(src, RIGHT_BRACE, "}"));
      break;
      case '[':
        jl_token_list_add(token_list, jl_token_new(src, LEFT_BRACKET, "["));
      break;
      case ']':
        jl_token_list_add(token_list, jl_token_new(src, RIGHT_BRACKET, "]"));
      break;
      case ',':
        jl_token_list_add(token_list,  jl_token_new(src, COMMA, ","));
      break;
      case ':':
        if(jl_source_code_peek(src, 1) == '^')
        {
          jl_token_list_add(token_list, jl_token_new(src, COLON_HAT, ":^"));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(src, COLON, ":"));
        }
      break;
      case '.':
        if(is_number(jl_source_code_peek(src, 1)))
        {
          jl_token_list_add(token_list, jl_token_new(src, NUMBER, consume_number(src)));
        }
        else if(jl_source_code_peek(src, 1) == '.')
        {
          jl_token_list_add(token_list, jl_token_new(src, DOT_DOT, ".."));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(src, DOT, "."));
        }
      break;
      case '-':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, MINUS_EQUAL, "-="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(src, MINUS, "-"));
        }
      break;
      case '+':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, PLUS_EQUAL, "+="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(src, PLUS, "+"));
        }
      break;
      case '%':
        jl_token_list_add(token_list,  jl_token_new(src, MODULUS, "%"));
      break;
      case '/':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, SLASH_EQUAL, "/="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(src, SLASH, "/"));
        }
      break;
      case '*':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, STAR_EQUAL, "*="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list,  jl_token_new(src, STAR, "*"));
        }
      break;
      case '\'':
      case '"':
        jl_token_list_add(token_list, jl_token_new(src, STRING_WRAPPER, consume_string(src)));
      break; 
      case '!':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, BANG_EQUAL, "!="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(src, BANG, "!")); 
        }
      break;
      case '=':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, EQUAL_EQUAL, "=="));
          jl_source_code_advance(src); 
        }
        else {
          jl_token_list_add(token_list, jl_token_new(src, EQUAL, "="));
        }
      break;
      case '>':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, GREATER_EQUAL, ">="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(src, GREATER, "<"));  
        }
      break;
      case '<':
        if(jl_source_code_peek(src, 1) == '=')
        {
          jl_token_list_add(token_list, jl_token_new(src, LESS_EQUAL, "<="));
          jl_source_code_advance(src);
        }
        else 
        {
          jl_token_list_add(token_list, jl_token_new(src, LESS, "<"));
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
        jl_token_list_add(token_list,  jl_token_new(src, NUMBER, consume_number(src)));
      break;
      case '\n':
      case ';':
        jl_token_list_add(token_list, jl_token_new(src, TERMINATOR, ";"));
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
          jl_token_list_add(token_list, consume_identifier(src));
          break;
        }
        jl_token_list_add(token_list, token);
    }
  }
  if(token_list->count > 0 && jl_token_list_peek(token_list, 0)->type != TERMINATOR)
  {
    jl_token_list_add(token_list, jl_token_new(src, TERMINATOR, ";"));
  }
  jl_token_list_add(token_list, jl_token_new(src, END_OF_FILE, "EOF"));
  return token_list;
}
