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
#include "debug.h"

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
  vm_push_frame(vm, stack_new(), true);
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
