#include "jlang_object.h"
#include "jlang_common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
 
jl_object_t *jl_new_int(int value)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = INT;
  obj->data.v_int = value;
  obj->refcount = 1;
  return obj;
}

jl_object_t *jl_new_float(float value)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = FLOAT;
  obj->data.v_float = value;
  obj->refcount = 1;
  return obj;
}

jl_object_t *jl_new_string(char *value)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  size_t size = strlen(value);
  obj->data.v_string = calloc(size, sizeof(char));
  if(obj->data.v_string == NULL)
  {
    return NULL;
  }
  strcpy(obj->data.v_string, value);
  obj->is_marked = false;
  obj->type = STRING;
  obj->refcount = 1;
  return obj;
}

jl_object_t *new_array(size_t size)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->data.v_array->elements = malloc(sizeof(jl_object_t) * size);
  if(obj->data.v_array->elements == NULL)
  {
    jl_object_free(obj);
    return NULL;
  }
  obj->is_marked = false;
  obj->type = ARRAY; 
  obj->refcount = 1;
  obj->data.v_array->size = size;
  return obj;
}

jl_object_t *jl_new_null()
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = NIL_OBJECT;
  obj->refcount = 1;
  return obj;
}

void jl_object_set_null(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  obj->type = NIL_OBJECT;
}

size_t jl_length(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return -1;
  }
  switch(obj->type)
  {
    case INT:
    case FLOAT:
      return 1;
      break;
    case STRING:
      return strlen(obj->data.v_string); 
      break;
    case ARRAY:
      return obj->data.v_array->size;
  }
  return -1;
}

jl_object_t *jl_add(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == FLOAT)
    {
      return jl_new_int(a->data.v_int + b->data.v_float);  
    }
    else if(b->type == INT)
    {
      return jl_new_int(a->data.v_int + b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_float(a->data.v_float + b->data.v_int); 
    }
    else if(b->type == FLOAT)
    {
      return jl_new_float(a->data.v_float + b->data.v_float);
    }
  }
  if(a->type == STRING && b->type == STRING)
  {
    size_t a_size = strlen(a->data.v_string);
    size_t b_size = strlen(b->data.v_string);
    char *str = calloc(a_size + b_size + 1, sizeof(char));
    strcpy(str, a->data.v_string);
    strcpy(str + a_size, b->data.v_string);
    jl_object_t * obj = jl_new_string(str);
    free(str);
    return obj;
  }
  return NULL;
}

jl_object_t *jl_subtract(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == FLOAT)
    {
      return jl_new_int(a->data.v_int - b->data.v_float);  
    }
    else if(b->type == INT)
    {
      return jl_new_int(a->data.v_int - b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_float(a->data.v_float - b->data.v_int); 
    }
    else if(b->type == FLOAT)
    {
      return jl_new_float(a->data.v_float - b->data.v_float);
    }
  }
  if(a->type == STRING)
  {
    if(b->type == STRING)
    {
      jl_strip_instances_from_string(a, b);
      return a;
    }
    else if(b->type == INT)
    {
      jl_shorten_string(a, b);
      return a;
    }
  }
  return NULL;
}

jl_object_t *jl_multiply(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == FLOAT)
    {
      return jl_new_int(a->data.v_int * b->data.v_float);  
    }
    else if(b->type == INT)
    {
      return jl_new_int(a->data.v_int * b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_float(a->data.v_float * b->data.v_int); 
    }
    else if(b->type == FLOAT)
    {
      return jl_new_float(a->data.v_float * b->data.v_float);
    }
  }
  if(a->type == STRING)
  {
    if(b->type == STRING)
    {
      printf("Hey! Thats  illegal!");
      return a;
    }
    else if(b->type == INT)
    {
      jl_multiply_string(a, b);
      return a;
    }
  }
  return NULL;
}

jl_object_t *jl_divide(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(a->data.v_int == 0)
    {
      printf("ERROR: divide by 0");
      return NULL;
    }
    if(b->type == FLOAT)
    {
      if(b->data.v_float == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_float);  
    }
    else if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
    if(a->data.v_float == 0)
    {
      printf("ERROR: divide by 0");
      return NULL;
    }
    if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      return jl_new_float(a->data.v_float / b->data.v_int); 
    }
    else if(b->type == FLOAT)
    {
      if(b->data.v_float == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
     return jl_new_float(a->data.v_float / b->data.v_float);
    }
  }
  if(a->type == STRING)
  {
    if(b->type == STRING)
    {
      printf("Hey! Thats  illegal!");
      return a;
    }
    else if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      jl_divide_string(a, b);
      return a;
    }
  }
  return NULL;
}

jl_object_t *jl_modulus(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(a->data.v_int == 0)
    {
      printf("ERROR: divide by 0");
      return NULL;
    }
    if(b->type == FLOAT)
    {
        printf("ERROR: divide by 0");
        return NULL;
    }
    else if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      return jl_new_int(a->data.v_int % b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
      printf("ERROR: divide by 0");
      return NULL;
  }
  if(a->type == STRING)
  {
    if(b->type == STRING)
    {
      printf("Hey! Thats  illegal!");
      return a;
    }
    else if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        printf("ERROR: divide by 0");
        return NULL;
      }
      jl_modulus_string(a, b);
      return a;
    }
  }
  return NULL;
}

void ref_free(jl_object_t *obj)
{
  if(obj == NULL || obj->refcount > 0)
  {
    return; 
  }
  switch(obj->type)
  {
    case INT:
    case FLOAT:
      jl_object_free(obj);
      break;
    case STRING:
      jl_object_free(obj);
      break;
    case ARRAY:
      for(size_t i = 0; i < obj->data.v_array->size; i++)
      {
        ref_dec(obj->data.v_array->elements[i]);
      }
      free(obj);
      break;
  }
}

void ref_dec(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  if(--obj->refcount == 0)
  {
    ref_free(obj);   
  }
}

void ref_inc(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  obj->refcount++;
}

void jl_array_set(jl_object_t *array, size_t index, jl_object_t *obj)
{
   
  if(array->data.v_array->size == 0 || index >= array->data.v_array->size)
  {
    return;
  }
  if(array->data.v_array->elements[index] != NULL)
  {
    ref_dec(array->data.v_array->elements[index]);
  }
  array->data.v_array->elements[index] = obj;
}

jl_object_t *jl_array_get(jl_object_t *array, size_t index)
{
  if(array->data.v_array->size == 0 || index >= array->data.v_array->size)
  {
    return NULL;
  }
  return array->data.v_array->elements[index];
}

void jl_object_free(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  switch(obj->type)
  {
    case INT:
    case FLOAT:
      break;
    case STRING:
      free(obj->data.v_string);
      break;
    case ARRAY:
      for(size_t i = 0; i < obj->data.v_array->size; i++)
      {
        jl_object_free(obj->data.v_array->elements[i]);
      }
  }
  free(obj);
}
