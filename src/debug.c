#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "jlang_token/jlang_token.h"
#include "jlang_token/jlang_token_list.h"
#include "jlang_program.h"
#include "jlang_syntax/jlang_syntax.h"


int count_syntax(jl_syntax_t *syntax, int num)
{ 
  if(syntax->token->type == NUMBER || syntax->token->type == STRING_OBJECT || syntax->token->type == IDENTIFIER)
  {
    printf("syntax level: %i token: %i literal: %s\n", num, syntax->token->type, syntax->token->literal); 
  }
  else
  {
    printf("syntax level: %i token: %i\n", num, syntax->token->type); 
  }
 
  num++;
  if(syntax->left != NULL)
  {
    count_syntax(syntax->left, num);
  }
  if(syntax->right != NULL)
  {
    count_syntax(syntax->right, num); 
  } 
  if(syntax->value != NULL)
  {
    count_syntax(syntax->value, num);
  }
  return num;
}

void count_tokens(jl_token_list_t *tokens)
{
  for(int i = 0; i < tokens->count; i++)
  {
    if(tokens->list[i]->type < 10)
    {
    printf("token[%i]->0%i-> %s\n", i, tokens->list[i]->type, tokens->list[i]->literal);
    }
    else 
    {
    printf("token[%i]->%i-> %s\n", i, tokens->list[i]->type, tokens->list[i]->literal);
    }
  }
  printf("token count: %i\n", tokens->count);
}

void debug(jl_token_list_t *tokens, jl_program_t *program)
{
  printf("tokens =======\n");
  count_tokens(tokens);
  for(int i = 0; i < program->count; i++)
  {
    printf("program: %i =======\n", i);
    count_syntax(program->statements[i], 0);
  }
}

void jl_print_object(jl_object_t *target)
{
  if(target == NULL)
  {
    return;
  }
  switch (target->type) {  
    case NULL_OBJECT:
      printf("Object: NULL");
    break;
    case INT_OBJECT:
     printf("Object: %i\n", target->data.v_int);
    break;
    case FLOAT_OBJECT:
     printf("Object: %d\n", target->data.v_float);
    break;
    case STRING_OBJECT:
     printf("Object: %s\n", target->data.v_string);
    break;
    case BOOL_OBJECT:
      if(target->data.v_bool == true)
      {
        printf("Object: true\n", target->data.v_string);
        break;
      }
      printf("Object: false\n", target->data.v_string);
    break;
    case ARRAY_OBJECT:
      printf("Object: array(%d)\n", target->data.v_array->count);
      print_array_content(target);
    break;
  }
}

void print_array_content(jl_object_t *arr)
{
  for(int i = 0; i < arr->data.v_array->count; i++)
  {
    printf("  [%i] ", i);
    jl_print_object(arr->data.v_array->elements[i]);
  }
}
