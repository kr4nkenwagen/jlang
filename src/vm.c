#include "vm.h"

vm_t *vm_new()
{
  vm_t *vm = malloc(sizeof(vm_t));
  if(vm == NULL)
  {
    return NULL;
  }
  vm->frames;
  return vm;
 }
