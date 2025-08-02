#include <stdio.h>
#include <string.h>
#include "jlang_object/jlang_object.h"
#include "stack.h"
#include <stdlib.h>

 stack_t *stack_new()
{
  stack_t *obj = malloc(sizeof(stack_t));
  if(obj == NULL)
  {
    printf("ddd@]\n");
    return NULL;
  }
  obj->capacity = 8;
  obj->count = 0;
  obj->parent_references = 0;
  obj->data = calloc(sizeof(void *), obj->capacity);
  if(obj->data == NULL)
  {
    printf("ddd]\n");
    free(obj);
    return NULL;
  }
  return obj;
}

void stack_push(stack_t *stack, jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  if(stack->count >= stack->capacity)
  {
    stack->capacity *= 2;
    if(realloc(stack->data, sizeof(jl_object_t *) * stack->capacity) == NULL)
    {
      return;
    }
  }
  stack->data[stack->count++] = obj;
}

jl_object_t *stack_pop(stack_t *stack)
{
  if(stack == NULL)
  {
    return NULL;
  }
  if(stack->count == 0)
  {
    return NULL;
  }
  return stack->data[stack->count--];
}

void stack_free(stack_t *stack)
{
  if(stack == NULL)
  {
    return;
  }
  if (stack->count > 0)
  {
    free(stack->data);
  }
  free(stack);
}

void stack_remove_nulls(stack_t *stack)
{
  size_t new_count = 0;
  for(size_t i = 0; i < stack->count; i++)
  {
    if(stack->data[i] != NULL)
    {
      stack->data[new_count++] = stack->data[i];
    }
  }
  stack->count = new_count;
}

jl_object_t *jl_stack_get(stack_t* stack, char *name)
{
  if(stack == NULL)
  {
    return NULL;
  }
  for(int i = 0; i < stack->count; i++)
  {
    if(strcmp(stack->data[i]->name, name) == 0)
    {
      return stack->data[i];
    }
  }
  return NULL;
}
