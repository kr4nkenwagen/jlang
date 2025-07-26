#include "jlang_object.h"
#include "jlang_common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>
 
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

jl_object_t *jl_new_bool(bool value)
{
  jl_object_t *obj = malloc(sizeof(jl_object_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->is_marked = false;
  obj->type = BOOLEAN;
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
  obj->type = ARRAY; 
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
  obj->type = VECTOR;
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

jl_object_t *jl_substring(jl_object_t *obj, int start, int length)
{
  if(obj == NULL || obj->type != STRING)
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

int jl_position_of_first_instance(jl_object_t *obj, char *instance)
{
  if(obj == NULL)
  {
    return -1;
  }
  if(obj->type == STRING)
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
  if(obj->type == STRING)
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
  if(a->type == STRING)
  {
    if(b->type == STRING)
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
  if(a->type == STRING)
  {
    if(b->type == STRING)
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

char *int_to_number(int num)
{
  size_t size = (int)log10(num) * sizeof(char) + 1;
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

void jl_array_set(jl_object_t *arr, int index, jl_object_t *obj)
{
   
  while(arr->data.v_array->size <= (size_t)index)
  {
    void **resized = jl_double_array_size(
      arr->data.v_array->elements, 
      arr->data.v_array->size
    );
    if(resized == NULL)
    {
      return;
    }
    arr->data.v_array->elements = (jl_object_t **)resized;
    arr->data.v_array->size *= 2;
  } 
  while(arr->data.v_array->count < index)
  {
    arr->data.v_array->elements[arr->data.v_array->count++] = jl_new_null();
  }
  arr->data.v_array->elements[index] = obj;
  if(arr->data.v_array->count == index)
  {
    arr->data.v_array->count++;
  }
}

jl_object_t *jl_array_get(jl_object_t *array, int index)
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
