#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scan.h"
#include "jlang_token.h"


source_code_t *open_src(char *src)
{
  FILE *file_ptr = fopen(src, "r");
  if(file_ptr == NULL)
  {
    return NULL;
  }
  source_code_t *src_code = malloc(sizeof(source_code_t));
  if(src_code == NULL)
  {
    return NULL;
  }
  fseek(file_ptr, 0L, SEEK_END);
  src_code->size = ftell(file_ptr);
  fseek(file_ptr, 0L, SEEK_SET);
  src_code->src = malloc(sizeof(char) * src_code->size);
  char *src_line= malloc(sizeof(char) * src_code->size);
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
  src_code->is_at_end == false;
  src_code->pointer = 0;
  src_code->line = 0;
  return src_code;
}

char advance(source_code_t *src)
{
  if(src == NULL || src->is_at_end)
  {
    return 0; 
  }
  if(src->pointer++ == src->size)
  {
    src->is_at_end = true;
  }
  size_t buffer_len = src->buffer ? strlen(src->buffer) : 0;
  char *new_buffer = realloc(src->buffer, buffer_len + 2);
  if(new_buffer == NULL)
  {
    return 0;
  }
  new_buffer[buffer_len] = src->src[src->pointer];
  new_buffer[buffer_len + 1] = '\0';
  src->buffer = new_buffer;
  if(src->src[src->pointer] == '\n')
  {
    src->line++;
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

void clear_buffer(source_code_t *src)
{
  if(src == NULL || src->buffer == NULL)
  {
    return;
  }
  free(src->buffer);
}

jl_token_list_t *scan(char *file)
{
  source_code_t *src = open_src(file);
  if(src == NULL)
  {
    return NULL;
  }
  jl_token_list_t *token_list = jl_token_list_new();
  while(!src->is_at_end)
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
        jl_token_list_add(token_list,  jl_token_new(DOT));
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
      case '\n':
      case '\t':
      case ' ':
      break;
      default:
       printf("ERROR: Unexpected character '%c' on line %i\n", src->src[src->pointer], src->line);
      break;
    }
  }
  return token_list;
}
