#include <stdio.h>
#include <string.h>
#include "scan.h"
#include "parse.h"
#include "info.h"
#include "jlang_token.h"

void repl()
{
  printf("%s[%s] - %s\nrepl\n", APPLICATION_NAME, VERSION, AUTHOR);
  while(true)
  {
    putchar('>');
    size_t size = 32;
    char *input = malloc(sizeof(char) * size);
    if(input == NULL)
    {
      return;
    }
    scanf("%s", input);
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
          return ;
        }
        memcpy(input, tmp_input, size / 2);
        free(tmp_input);
      }
    }
    if(strcmp(input, "exit") == 0)
    {
      return;
    }

    parse(scan(from_repl_line(input)));
  }
}
