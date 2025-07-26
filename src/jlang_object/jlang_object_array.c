#include <string.h>
#include "jlang_object.h"

void **jl_double_array_size(void **arr, size_t curr_size)
{
  if(arr == NULL)
  {
    return NULL;
  }
  size_t new_size = curr_size * 2;
  void **new_arr = malloc(new_size * sizeof(void *));
  if(new_arr == NULL)
  {
    return NULL;
  }
  memcpy(new_arr, arr, curr_size * sizeof(void *));
  free(arr);
  return new_arr;
}

void jl_array_set(jl_object_t *arr, int index, jl_object_t *obj)
{
  while(arr->data.v_array->size <= (size_t)index)
  {
    void **resized = jl_double_array_size(
      (void **)arr->data.v_array->elements, 
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
