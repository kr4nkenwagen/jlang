#include "vm.h"
#include "stack.h"

vm_t *vm_new()
{
  vm_t *vm = malloc(sizeof(vm_t));
  if(vm == NULL)
  {
    return NULL;
  }
  vm->size = 8;
  vm->count = 0;
  vm->frames = malloc(sizeof(sizeof(stack_t) * vm->size));
  if(vm->frames == NULL)
  {
    return NULL;
  }
  return vm;
}

void vm_push_frame(vm_t *vm, stack_t *frame)
{
  if(vm == NULL)
  {
    return;
  }
  if(vm->count > vm->size)
  {
    stack_t **tmp = malloc(sizeof(stack_t) * vm->size * 2);
    memcpy(tmp, vm->frames, vm->size);
    free(vm->frames);
    vm->frames = tmp;
    vm->size *= 2;
  }
  vm->frames[vm->count++] = frame;
}

stack_t *vm_pop_frame(vm_t *vm)
{
  if(vm == NULL || vm->count == 0)
  {
    return NULL;
  }
  return (stack_t *)vm->frames[vm->count--];
}

stack_t *vm_curr_frame(vm_t *vm)
{
  if(vm == NULL || vm->count == 0)
  {
    return NULL;
  }
  return (stack_t *)vm->frames[vm->count - 1];
}
