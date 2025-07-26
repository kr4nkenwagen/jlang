#include <stdlib.h>
#include "stack.h"
#include "vm.h"
#include "jlang_object/jlang_object.h"

void mark(vm_t *vm)
{
  for(size_t i = 0; i < vm->objects->count; i++)
  {
    jl_object_t *obj = vm->objects->data[i];
    if(obj->is_marked)
    {
      obj->is_marked = false;
      continue;
    }
    jl_object_free(obj);
    vm->objects->data[i] = NULL;
  }
  stack_remove_nulls(vm->objects);
}

void trace(vm_t *vm)
{
  stack_t *gray_objs = stack_new();
  if(gray_objs == NULL)
  {
    return;
  }
  for(size_t i = 0; i < vm->objects->count; i++)
  {
    jl_object_t *obj = vm->objects->data[i];
    if(obj->is_marked)
    {
      stack_push(gray_objs, obj);
    }
  }

  while(gray_objs->count > 0)
  {
    jl_object_t *obj = stack_pop(gray_objs);
    switch (obj->type)
    {
      case INT:
      case FLOAT:
      case STRING:
        break;
      case ARRAY:
        for(size_t i = 0; i < obj->data.v_array->size; i++)
        {
          if(obj->data.v_array->elements[i]->is_marked)
          {
            continue;
          }
          stack_push(gray_objs, obj->data.v_array->elements[i]);
          obj->data.v_array->elements[i]->is_marked = true;
        }
      break;
    }
  }
}

void sweep(vm_t *vm)
{

}
