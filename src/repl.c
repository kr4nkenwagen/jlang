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
    char *input;
    scanf("%s", input);
    
    if(strcmp(input, "exit") == 0)
    {
      return;
    }

    parse(scan(from_repl_line(input)));
  }
}
