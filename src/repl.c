#include <stdio.h>
#include <string.h>
#include "scan.h"
#include "jlang_parser/jlang_parser.h"
#include "jlang_interpreter/jlang_interpreter.h"
#include "info.h"
#include "jlang_source_code.h"
#include "jlang_token/jlang_token.h"
#include "jlang_syntax/jlang_syntax.h"
#include "jlang_program.h"
#include "stack.h"

//*******************
//* DEBUG FUNCTIONS *
//*******************
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

char *user_input()
{
  size_t size = 32;
  char *input = malloc(sizeof(char) * size);
  if(input == NULL)
  {
    return NULL;
  }
  char x;
  int count = 0;
  while(x = getchar())
  {
    if(x == '\n')
    {
      input[count] == '\0';
      fflush(stdout); 
      break;
    }
    input[count++] = x;
    if(count == size)
    {
      char *tmp_input = malloc(sizeof(char) * size);
      memcpy(tmp_input, input, size);
      size *= 2;
      input = malloc(sizeof(char) * size);
      if(input == NULL)
      {
        return NULL;
      }
      memcpy(input, tmp_input, size / 2);
      free(tmp_input);
    }
  }
  return input;
}

void repl()
{
  printf("%s[%s] - %s\nrepl\n", APPLICATION_NAME, VERSION, AUTHOR);
  vm_t* vm = vm_new();
  vm_push_frame(vm, stack_new());
  while(true)
  {
    putchar('>');
    char * input = user_input();
    if(strcmp(input, "exit") == 0)
    {
      return;
    }
    jl_source_code_t *src = jl_source_code_from_repl(input);
    jl_token_list_t *tokens = scan(src);
    jl_program_t *program = parse(tokens);
   // debug(tokens, program);
    interprete(program, vm);
  }
}
