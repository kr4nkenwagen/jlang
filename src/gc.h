#include "stack.h"
#include "vm.h"

void mark(vm_t *vm);
void trace(vm_t *vm);
void sweep(vm_t *vm);
