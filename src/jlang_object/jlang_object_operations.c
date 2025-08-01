#include <stdio.h>
#include "jlang_object.h"
#include "jlang_object_operations.h"
#include "jlang_object_string.h"

jl_object_t *jl_add(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT_OBJECT)
  {
    if(b->type == FLOAT_OBJECT)
    {
      return jl_new_int(a->data.v_int + b->data.v_float);  
    }
    else if(b->type == INT_OBJECT)
    {
      return jl_new_int(a->data.v_int + b->data.v_int);
    }
    if(b->type == STRING_OBJECT)
    {
      char *num = int_to_number(a->data.v_int);
      return jl_new_string(join_string(num, b->data.v_string));
    }
  }
  if(a->type == FLOAT_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
      return jl_new_float(a->data.v_float + b->data.v_int); 
    }
    else if(b->type == FLOAT_OBJECT)
    {
      return jl_new_float(a->data.v_float + b->data.v_float);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == STRING_OBJECT)
    {
      return jl_new_string(join_string(a->data.v_string, b->data.v_string));
    }
    else if(b->type == INT_OBJECT)
    {
      char *num = int_to_number(b->data.v_int);
      return jl_new_string(join_string(a->data.v_string, num));
    }
  }
  return NULL;
}

jl_object_t *jl_subtract(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT_OBJECT)
  {
    if(b->type == FLOAT_OBJECT)
    {
      return jl_new_int(a->data.v_int - b->data.v_float);  
    }
    else if(b->type == INT_OBJECT)
    {
      return jl_new_int(a->data.v_int - b->data.v_int);
    }
  }
  if(a->type == FLOAT_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
      return jl_new_float(a->data.v_float - b->data.v_int); 
    }
    else if(b->type == FLOAT_OBJECT)
    {
      return jl_new_float(a->data.v_float - b->data.v_float);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == STRING_OBJECT)
    {
      jl_strip_instances_from_string(a, b);
      return a;
    }
    else if(b->type == INT_OBJECT)
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
  if(a->type == INT_OBJECT)
  {
    if(b->type == FLOAT_OBJECT)
    {
      return jl_new_int(a->data.v_int * b->data.v_float);  
    }
    else if(b->type == INT_OBJECT)
    {
      return jl_new_int(a->data.v_int * b->data.v_int);
    }
  }
  if(a->type == FLOAT_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
      return jl_new_float(a->data.v_float * b->data.v_int); 
    }
    else if(b->type == FLOAT_OBJECT)
    {
      return jl_new_float(a->data.v_float * b->data.v_float);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == INT_OBJECT)
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
  if(a->type == INT_OBJECT)
  {
    if(a->data.v_int == 0)
    {
      return NULL;
    }
    if(b->type == FLOAT_OBJECT)
    {
      if(b->data.v_float == 0)
      {
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_float);  
    }
    else if(b->type == INT_OBJECT)
    {
      if(b->data.v_int == 0)
      {
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_int);
    }
  }
  if(a->type == FLOAT_OBJECT)
  {
    if(a->data.v_float == 0)
    {
      return NULL;
    }
    if(b->type == INT_OBJECT)
    {
      if(b->data.v_int == 0)
      {
        return NULL;
      }
      return jl_new_float(a->data.v_float / b->data.v_int); 
    }
    else if(b->type == FLOAT_OBJECT)
    {
      if(b->data.v_float == 0)
      {
        return NULL;
      }
     return jl_new_float(a->data.v_float / b->data.v_float);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
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
  if(a->type == INT_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
      return jl_new_int(a->data.v_int % b->data.v_int);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == INT_OBJECT)
    {
      jl_modulus_string(a, b);
      return a;
    }
  }
  return NULL;
}

void jl_assign(jl_object_t *target, jl_object_t *source)
{
  if(target == NULL || source == NULL)
  {
    return;
  }
  if(target->is_const == true)
  {
    return;
  }
  target->data = source->data;
  target->type = source->type;
}

void ref_free(jl_object_t *obj)
{
  if(obj == NULL || obj->refcount > 0)
  {
    return; 
  }
  switch(obj->type)
  {
    case INT_OBJECT:
    case FLOAT_OBJECT:
      jl_object_free(obj);
      break;
    case STRING_OBJECT:
      jl_object_free(obj);
      break;
    case ARRAY_OBJECT:
      for(size_t i = 0; i < obj->data.v_array->size; i++)
      {
        ref_dec(obj->data.v_array->elements[i]);
      }
      free(obj);
      break;
  }
}
