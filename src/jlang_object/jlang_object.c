#include "jlang_object.h"
#include "../debug.h"
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
  obj->type = INT_OBJECT;
  obj->data.v_int = value;
  obj->refcount = 1;
  return obj;
}

jl_object_t *jl_new_bool(bool value)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = BOOL_OBJECT;
  obj->data.v_bool = value;
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
  obj->type = FLOAT_OBJECT;
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
  obj->type = STRING_OBJECT;
  obj->refcount = 1;
  return obj;
}

jl_object_t *jl_new_array()
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->data.v_array = malloc(sizeof(jl_array_t));
  if(obj->data.v_array == NULL)
  {
    return NULL;
  }
   obj->data.v_array->elements = malloc(sizeof(jl_object_t *) * 8);
  if(obj->data.v_array->elements == NULL)
  {
    jl_object_free(obj);
    return NULL;
  }
  obj->is_marked = false;
  obj->type = ARRAY_OBJECT; 
  obj->data.v_array->count = 0;
  obj->refcount = 1;
  obj->data.v_array->size = 8;
  return obj;
}

jl_object_t *jl_new_vector(jl_object_t *x, jl_object_t *y, jl_object_t *z)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = VECTOR_OBJECT;
  obj->refcount = 1;
  obj->data.v_vector->x = x;
  obj->data.v_vector->y = y;
  obj->data.v_vector->z = z;
  return obj;
}

jl_object_t *jl_new_funct(void *syntax)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = FUNCTION_OBJECT;
  obj->refcount = 1;
  obj->data.v_funct = syntax;
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
  obj->type = NULL_OBJECT;
  obj->refcount = 1;
  return obj;
}

void jl_object_set_null(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  obj->type = NULL_OBJECT;
}

size_t jl_length(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return -1;
  }
  switch(obj->type)
  {
    case INT_OBJECT:
    case FLOAT_OBJECT:
      return 1;
      break;
    case STRING_OBJECT:
      return strlen(obj->data.v_string); 
      break;
    case ARRAY_OBJECT:
      return obj->data.v_array->size;
  }
  return -1;
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


void jl_object_free(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  switch(obj->type)
  {
    case INT_OBJECT:
    case FLOAT_OBJECT:
      break;
    case STRING_OBJECT:
      free(obj->data.v_string);
      break;
    case ARRAY_OBJECT:
      for(size_t i = 0; i < obj->data.v_array->size; i++)
      {
        jl_object_free(obj->data.v_array->elements[i]);
      }
  }
  free(obj);
}
