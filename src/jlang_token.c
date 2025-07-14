#include <string.h>
#include "jlang_token.h"

jl_token_list_t *jl_token_list_new()
{
  jl_token_list_t *list = malloc(sizeof(jl_token_list_t));
  if(list == NULL)
  {
    return NULL;
  }
  list->size = 8;
  list->count = 0;
  list->index = 0;
  list->list = malloc(sizeof(jl_token_t *) * list->size);
  if(list->list == NULL)
  {
    free(list);
    return NULL;
  }
  return list;
}

void jl_token_list_add(jl_token_list_t *list, jl_token_t *token)
{
  if(token == NULL || list == NULL)
  {
    return;
  }
  if(list->count == list->size)
  {
    list->size *= 2;
    jl_token_t **new_list = malloc(sizeof(jl_token_t **) * list->size);
    if(new_list == NULL)
    {
      return;
    }
    memcpy(new_list, list->list, sizeof(jl_token_t **) * (list->size / 2));
    free(list->list);
    list->list = new_list;
  }
  list->list[list->count++] = token;
}

jl_token_t *jl_token_list_advance(jl_token_list_t *list)
{
  if(list->index >= list->count - 1)
  {
    return NULL;
  }
  return list->list[++list->index];
}

jl_token_t *jl_token_list_peek(jl_token_list_t *list, int distance)
{
  if(list->index + distance >= list->count)
  {
    return NULL;
  }
  return list->list[list->index + distance];
}

jl_token_t *jl_token_new(jl_token_type_t type)
{
  jl_token_t *token = malloc(sizeof(jl_token_t));
  if(token == NULL)
  {
    return NULL;
  }
  token->type = type;

  return token;
}
