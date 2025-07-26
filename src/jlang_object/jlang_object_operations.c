#include "jlang_object.h"
#include "jlang_object_operations.h"
#include "jlang_object_string.h"

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
    if(b->type == STRING)
    {
      char *num = int_to_number(a->data.v_int);
      return jl_new_string(join_string(num, b->data.v_string));
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
  if(a->type == STRING)
  {
    if(b->type == STRING)
    {
      return jl_new_string(join_string(a->data.v_string, b->data.v_string));
    }
    else if(b->type == INT)
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
    if(b->type == INT)
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
      return NULL;
    }
    if(b->type == FLOAT)
    {
      if(b->data.v_float == 0)
      {
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_float);  
    }
    else if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        return NULL;
      }
      return jl_new_int(a->data.v_int / b->data.v_int);
    }
  }
  if(a->type == FLOAT)
  {
    if(a->data.v_float == 0)
    {
      return NULL;
    }
    if(b->type == INT)
    {
      if(b->data.v_int == 0)
      {
        return NULL;
      }
      return jl_new_float(a->data.v_float / b->data.v_int); 
    }
    else if(b->type == FLOAT)
    {
      if(b->data.v_float == 0)
      {
        return NULL;
      }
     return jl_new_float(a->data.v_float / b->data.v_float);
    }
  }
  if(a->type == STRING)
  {
    if(b->type == INT)
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
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_int(a->data.v_int % b->data.v_int);
    }
  }
  if(a->type == STRING)
  {
    if(b->type == INT)
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
