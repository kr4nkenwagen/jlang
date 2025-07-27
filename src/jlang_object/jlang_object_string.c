#include <string.h>
#include <stdio.h>
#include "jlang_object.h"

int int_length(int i)
{
  if(i >= 1000000000) return 10;
  if(i >= 100000000) return 9;
  if(i >= 10000000) return 8;
  if(i >= 1000000) return 7;
  if(i >= 100000) return 6;
  if(i >= 10000) return 5;
  if(i >= 1000) return 4;
  if(i >= 100) return 3;
  if(i >= 10) return 2;
  return 1;
}

char *int_to_number(int num)
{
  size_t size = int_length(num);
  char *word = malloc(size);
  sprintf(word, "%d", num);
  return word;
}

char *join_string(char *a, char *b)
{
  size_t a_size = strlen(a);
  size_t b_size = strlen(b);
  char *str = calloc(a_size + b_size + 1, sizeof(char));
  strcpy(str, a);
  strcpy(str + a_size, b);
  return str;
}

int jl_position_of_first_instance(jl_object_t *obj, char *instance)
{
  if(obj == NULL)
  {
    return -1;
  }
  if(obj->type == STRING_OBJECT)
  {
    int position = 0;
    size_t instance_size = strlen(instance);
    size_t obj_size = jl_length(obj);
    while(position + instance_size <= obj_size)
    {
      bool is_same = true;
      for(int i = 0; i < instance_size; i++)
      {
        if(obj->data.v_string[position + i] != instance[i])
        {
          is_same = false;
          break;
        }
      }
      if(is_same)
      {
        return position;
      }
      position++;
    }
  }
  return -1;
}

int jl_position_of_last_instance(jl_object_t *obj, char *instance)
{
  if(obj == NULL)
  {
    return -1;
  }
  if(obj->type == STRING_OBJECT)
  {
    int position = 0;
    size_t instance_size = strlen(instance);
    size_t obj_size = jl_length(obj);
    int instance_position = -1;
    while(position + instance_size <= obj_size)
    {
      bool is_same = true;
      for(int i = 0; i < instance_size; i++)
      {
        if(obj->data.v_string[position + i] != instance[i])
        {
          is_same = false;
          break;
        }
      }
      if(is_same)
      {
        instance_position = position;
      }
      position++;
    }
    return instance_position;
  }
  return -1;
}

jl_object_t *jl_substring(jl_object_t *obj, int start, int length)
{
  if(obj == NULL || obj->type != STRING_OBJECT)
  {
    return NULL;
  }
  size_t size = jl_length(obj);
  if(length == -1)
  {
    length = size - start;
  }
  if(start + length > size)
  {
    return NULL;
  }
  char *substring = malloc(sizeof(char) * length);
  memcpy(substring, obj->data.v_string + start, length);
  substring[length] = '\0';
  return jl_new_string(substring);
}

int jl_string_length(jl_object_t *target)
{
  if(target == NULL || target->type != STRING_OBJECT)
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
  if(target == NULL || instance == NULL || target->type != STRING_OBJECT || instance->type != STRING_OBJECT)
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
  if(target == NULL || length <= 0 || target->type != STRING_OBJECT || length->type != INT)
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
  if(target == NULL || multiplier == NULL || target->type != STRING_OBJECT || multiplier->type != INT)
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
  if(target == NULL || divider == NULL || target->type != STRING_OBJECT || divider->type != INT)
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
  if(target == NULL || modulus == NULL || target->type != STRING_OBJECT || modulus->type != INT)
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


