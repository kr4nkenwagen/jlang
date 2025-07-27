#include <string.h>
#include "jlang_object.h"

jl_object_t *jl_equals(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int == b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int == b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int == b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float == b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float == b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float == b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool == b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool == b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool == b->data.v_bool);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == STRING_OBJECT)
    {
      return jl_new_bool(strcmp(a->data.v_string, b->data.v_string) == 0);
    }
  }
  if(a->type == NIL_OBJECT)
  {
    if(b->type == NIL_OBJECT)
    {
      return jl_new_bool(true);
    }
  }
  return NULL;
}

jl_object_t *jl_not_equals(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int != b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int != b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int != b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float != b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float != b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float != b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool != b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool != b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool != b->data.v_bool);
    }
  }
  if(a->type == STRING_OBJECT)
  {
    if(b->type == STRING_OBJECT)
    {
      return jl_new_bool(strcmp(a->data.v_string, b->data.v_string) != 0);
    }
  }
  if(a->type == NIL_OBJECT)
  {
    if(b->type == NIL_OBJECT)
    {
      return jl_new_bool(true);
    }
  }
  return NULL;
}

jl_object_t *jl_greater_equals(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int >= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int >= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int >= b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float >= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float >= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float >= b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool >= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool >= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool >= b->data.v_bool);
    }
  }
  if(a->type == NIL_OBJECT)
  {
    if(b->type == NIL_OBJECT)
    {
      return jl_new_bool(true);
    }
  }
  return NULL;
}

jl_object_t *jl_greater(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int > b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int > b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int > b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float > b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float > b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float > b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool > b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool > b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool > b->data.v_bool);
    }
  }
  return NULL;
}

jl_object_t *jl_less(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int < b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int < b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int < b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float < b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float < b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float < b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool < b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool < b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool < b->data.v_bool);
    }
  }
  return NULL;
}

jl_object_t *jl_less_equals(jl_object_t *a, jl_object_t *b)
{
  if(a == NULL || b == NULL)
  {
    return NULL;
  }
  if(a->type == INT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_int <= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_int <= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_int <= b->data.v_bool);
    }
  }
  if(a->type == FLOAT)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_float <= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_float <= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_float <= b->data.v_bool);
    }
  }
  if(a->type == BOOLEAN)
  {
    if(b->type == INT)
    {
      return jl_new_bool(a->data.v_bool <= b->data.v_int);
    }
    if(b->type == FLOAT)
    {
      return jl_new_bool(a->data.v_bool <= b->data.v_float);
    }
    if(b->type == BOOLEAN)
    {
      return jl_new_bool(a->data.v_bool <= b->data.v_bool);
    }
  }
  return NULL;
}


