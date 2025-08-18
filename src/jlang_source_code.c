#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jlang_source_code.h"

jl_source_code_t *jl_source_code_new()
{
  jl_source_code_t *src = malloc(sizeof(jl_source_code_t));
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

jl_source_code_t *jl_source_code_from_file(char *src)
{
  FILE *file_ptr = fopen(src, "r");
  if(file_ptr == NULL)
  {
    return NULL;
  }
  jl_source_code_t *src_code = jl_source_code_new(); 
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

void jl_source_code_import_file(jl_source_code_t *target, char *src)
{
  jl_source_code_t *obj = jl_source_code_from_file(src);
  char *tmp = (char*)calloc(sizeof(char), target->size + obj->size + 2);
  if(tmp == NULL)
  {
    return;
  }
  memcpy(tmp,target->src, target->pointer);
  tmp[target->pointer] = '\n';
  memcpy(tmp + target->pointer + 1, obj->src, obj->size);
  tmp[target->pointer + obj->size + 1] = '\n';
  memcpy(tmp + target->pointer + obj->size + 2, target->src + target->pointer, target->size - target->pointer);
  target->size += obj->size + 2;
  tmp[target->size] = '\0';
  free(target->src);
  target->src = tmp;
  free(obj);
}

jl_source_code_t *jl_source_code_from_repl(char *line)
{
  if(line == NULL)
  {
    return NULL;
  }
  jl_source_code_t *src = jl_source_code_new();
  if(src == NULL)
  {
    return NULL;
  }
  src->size = strlen(line); 
  src->src = malloc(src->size + 1);
  memcpy(src->src, line, src->size);
  src->src[src->size] = '\0';
  return src;
}

char jl_source_code_advance(jl_source_code_t *src)
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

char jl_source_code_peek(jl_source_code_t *src, int distance)
{
  if(src == NULL || src->pointer + distance >= src->size)
  {
    return 0;
  }
  return src->src[src->pointer + distance];
}


