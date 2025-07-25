#include <stdlib.h>
#include "jlang_object.h"

typedef struct stack {
  size_t count;
  int parent_references;
  size_t capacity;
  jl_object_t **data;
}stack_t;

stack_t *stack_new();
void stack_push(stack_t *stack, jl_object_t *obj);
jl_object_t *stack_pop(stack_t *stack);
void stack_free(stack_t *stack); 
void stack_remove_nulls(stack_t *stack);
jl_object_t *jl_stack_get(stack_t *, char *name);
