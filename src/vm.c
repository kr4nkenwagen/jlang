#include "vm.h"
#include "stdio.h"
#include "stack.h"

stack_t *vm_curr_frame(vm_t *vm);

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

void copy_references(stack_t *target, stack_t *src)
{
  for(int i = 0; i < src->count; i++) 
  {
    stack_push(target, src->data[i]);
  }
}

void vm_push_frame(vm_t *vm, stack_t *frame, bool inherit_stack)
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
  if(inherit_stack && vm->count > 0)
  {
    copy_references(frame, vm_curr_frame(vm)); 
    frame->parent_references = frame->count;
  }
  vm->frames[vm->count++] = frame;
}

void vm_pop_frame(vm_t *vm)
{
  if(vm == NULL || vm->count == 0)
  {
    return;
  }
  stack_t *frame = vm_curr_frame(vm);
  for(int i = frame->parent_references; i < frame->count; i++)
  {
    free(frame->data[i]);
  }
  free(frame);
  vm->count--;
}

stack_t *vm_curr_frame(vm_t *vm)
{
  if(vm == NULL || vm->count == 0)
  {
    return NULL;
  }
  return (stack_t *)vm->frames[vm->count - 1];
}
