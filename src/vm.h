#include <stdlib.h>
#include "stack.h"

typedef struct vm {
  stack_t *frames;
  stack_t *objects;
}vm_t;

typedef struct frame {
  stack_t references;
}frame_t;

vm_t *vm_new();
void vm_push_frame(vm_t *vm);
frame_t *vm_pop_frame(vm_t *vm);


