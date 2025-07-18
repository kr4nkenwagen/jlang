#include <stdio.h>
#include <string.h>
#include "jlang_object.h"

int jl_string_length(jl_object_t *target)
{
  if(target == NULL || target->type != STRING)
  {
    return -1;
  }
  int result = 0;
  while(target->data.v_string[result] != '\0')
  {
    result++;
  }
  return result;
}

void jl_strip_instances_from_string(jl_object_t *target, jl_object_t *instance)
{
  if(target == NULL || instance == NULL || target->type != STRING || instance->type != STRING)
  {
    return;
  }
  for(int i = 0; target->data.v_string[i] != '\0'; i++)
  {
    for(int x = 0; target->data.v_string[i + x] == instance->data.v_string[x]; x++)
    {
      if(instance->data.v_string[x + 1] == '\0')
      {
        x++;
        size_t target_size = jl_string_length(target);
        char *tmp_string = malloc(sizeof(char) * (target_size - x));
        if(tmp_string == NULL)
        {
          return;
        }
        memcpy(tmp_string, target->data.v_string, i);
        memcpy(tmp_string + i, target->data.v_string + i + x, target_size - i - x);
        free(target->data.v_string);
        target->data.v_string = tmp_string;
        target->data.v_string[target_size - x] = '\0';
      }
    }
  }
}

void jl_shorten_string(jl_object_t *target, jl_object_t *length)
{
  if(target == NULL || length <= 0 || target->type != STRING || length->type != INT)
  {
    return;
  }
  size_t new_size = jl_string_length(target) - length->data.v_int;
  char * tmp_string = malloc(sizeof(char) * new_size);
  memcpy(tmp_string, target->data.v_string, new_size);
  free(target->data.v_string);
  target->data.v_string = tmp_string;
  target->data.v_string[new_size] = '\0';
}

void jl_multiply_string(jl_object_t *target, jl_object_t *multiplier)
{
  if(target == NULL || multiplier == NULL || target->type != STRING || multiplier->type != INT)
  {
    return;
  }
  size_t old_size = jl_string_length(target); 
  size_t new_size = old_size * multiplier->data.v_int;
  char *tmp_string = malloc(sizeof(char) * new_size);
  for(int i = 0; i < multiplier->data.v_int; i++)
  {
    memcpy(tmp_string + (old_size * i), target->data.v_string, old_size);
  }
  free(target->data.v_string);
  target->data.v_string = tmp_string;
  target->data.v_string[new_size] = '\0';
}

void jl_divide_string(jl_object_t *target, jl_object_t *divider)
{
  if(target == NULL || divider == NULL || target->type != STRING || divider->type != INT)
  {
    return;
  }
  size_t old_size = jl_string_length(target); 
  size_t new_size = old_size / divider->data.v_int;
  char *tmp_string = malloc(sizeof(char) * new_size);
  memcpy(tmp_string, target->data.v_string, old_size);
  free(target->data.v_string);
  target->data.v_string = tmp_string;
  target->data.v_string[new_size] = '\0';
}

void jl_modulus_string(jl_object_t *target, jl_object_t *modulus)
{
  if(target == NULL || modulus == NULL || target->type != STRING || modulus->type != INT)
  {
    return;
  }
  size_t old_size = jl_string_length(target); 
  size_t new_size = old_size % modulus->data.v_int;
  char *tmp_string = malloc(sizeof(char) * new_size);
  memcpy(tmp_string, target->data.v_string+ (old_size - new_size), new_size);
  free(target->data.v_string);
  target->data.v_string = tmp_string;
  target->data.v_string[new_size] = '\0';
}


void jl_print_object(jl_object_t *target)
{
  if(target == NULL)
  {
    return;
  }
  switch (target->type) {  
    case NIL_OBJECT:
      printf("Object: NULL");
    break;
    case INT:
     printf("Object: %i\n", target->data.v_int);
    break;
    case FLOAT:
     printf("Object: %d\n", target->data.v_float);
    break;
    case STRING:
     printf("Object: %s\n", target->data.v_string);
    break;
  }
}
