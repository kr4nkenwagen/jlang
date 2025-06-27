#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scan.h"


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
  while(fgets(src_line, src_code->sizsrc_code->sizee, file_ptr))
  {
    strcat(src_code->src, src_line);     
  }
  free(src_line);
  src_code->is_at_end == false;
  src_code->pointer = 0;
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

void scan(char *src)
{
  source_code_t *raw_file_content = open_src(src);
  if(raw_file_content == NULL)
  {
    return NULL;
  }
  while(!src->is_at_end)
  {
    switch(advance(src))
    {

    }
  }
}
