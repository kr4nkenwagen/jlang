#include <stdio.h>
#include <string.h>
#include "scan.h"
#include "parse.h"
#include "interprete.h"
#include "info.h"
#include "jlang_source_code.h"
#include "jlang_token.h"
#include "jlang_syntax.h"
#include "jlang_program.h"

//*******************
//* DEBUG FUNCTIONS *
//*******************
int count_syntax(jl_syntax_t *syntax, int num)
{ 
  if(syntax->token->type == NUMBER)
  {
    if(syntax->value->type == INT)
    {
      printf("syntax level: %i token: %i value: %i\n", num, syntax->token->type, syntax->value->data.v_int); 
    }
    else
    {
      printf("syntax level: %i token: %i value: %i\n", num, syntax->token->type, syntax->value->data.v_float); 
    }
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
  if(syntax->next != NULL)
  {
    count_syntax(syntax->next, num - 1);
  } }
  return num;
}
void count_tokens(jl_token_list_t *tokens)
{
  for(int i = 0; i < tokens->count; i++)
  {
    printf("token[%i]->%i\n", i, tokens->list[i]->type);
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
    interprete(program);
    debug(tokens, program);
  }
}
