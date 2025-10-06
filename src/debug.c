#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "jlang_token/jlang_token.h"
#include "jlang_token/jlang_token_list.h"
#include "jlang_program.h"
#include "jlang_syntax/jlang_syntax.h"
#include "stack.h"


int count_syntax(jl_syntax_t *syntax, int num)
{ 
  printf("syntax level: %i token: %i literal: %s\n", num, syntax->token->type, syntax->token->literal); 
 
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

const char *jl_token_type_to_string(int type) 
{
  switch(type) 
  {
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case STRING_WRAPPER: return "STRING";
    case TRUE: return "TRUE";
    case FALSE: return "FALSE";
    case NIL: return "NIL";
    case PLUS: return "PLUS (+)";
    case MINUS: return "MINUS (-)";
    case STAR: return "STAR (*)";
    case SLASH: return "SLASH (/)";
    case MODULUS: return "MODULUS (%)";
    case DOT_DOT: return "STRING_ADD (..)";
    case EQUAL_EQUAL: return "EQUAL_EQUAL (==)";
    case BANG_EQUAL: return "NOT_EQUAL (!=)";
    case GREATER: return "GREATER (>)";
    case GREATER_EQUAL: return "GREATER_EQUAL (>=)";
    case LESS: return "LESS (<)";
    case LESS_EQUAL: return "LESS_EQUAL (<=)";
    case EQUAL: return "ASSIGN (=)";
    case PLUS_EQUAL: return "PLUS_ASSIGN (+=)";
    case MINUS_EQUAL: return "MINUS_ASSIGN (-=)";
    case STAR_EQUAL: return "STAR_ASSIGN (*=)";
    case SLASH_EQUAL: return "SLASH_ASSIGN (/=)";
    case BANG: return "BANG (!)";
    case COLON: return "COLON (:)";
    case COLON_HAT: return "COLON_HAT (:^)";
    case LEFT_PAREN: return "LEFT_PAREN";
    case RIGHT_PAREN: return "RIGHT_PAREN";
    default: return "OTHER";
  }
}

void jl_syntax_pretty_print(jl_syntax_t *syntax, int indent) {
  if (syntax == NULL) 
  {
    return;
  }
  for (int i = 0; i < indent; i++) 
  {
    printf("  ");
  }
  if (syntax->token) 
  {
   const char *type_name = jl_token_type_to_string(syntax->token->type);
    if (syntax->token->literal) 
    {
      printf("%s: '%s'\n", type_name, syntax->token->literal);
    } 
    else 
    {
      printf("%s\n", type_name);  
    }
  } 
  else 
  {
    printf("UNKNOWN syntax\n");
  }

  if (syntax->left || syntax->right) 
  {
    jl_syntax_pretty_print(syntax->left, indent + 1);
    jl_syntax_pretty_print(syntax->right, indent + 1);
  }
}

void jl_print_stack(stack_t *stack)
{
  printf("=stack=====\n");
  for(int i = 0; i < stack->count; i++)
  {
    printf(" %s\n", stack->data[i]->name);
  }
}

