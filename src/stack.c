#include "stack.h"
#include <stdlib.h>

 stack_t *stack_new(size_t capacity)
{
  stack_t *obj = malloc(sizeof(stack_t));
  if(obj == NULL)
  {
    return NULL;
  }
  obj->capacity = capacity;
  obj->count = 0;
  obj->data = calloc(sizeof(void *), capacity);
  if(obj->data == NULL)
  {
    return NULL;
  }
  return obj;
}

void stack_push(stack_t *stack, void *obj)
{
  if(obj == NULL)
  {
    return;
  }
  if(stack->count >= stack->capacity)
  {
    stack->capacity *= 2;
    realloc(stack->data, sizeof(void *) * stack->capacity);
  }
  stack->data[stack->count++] = obj;
}

void *stack_pop(stack_t *stack)
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
