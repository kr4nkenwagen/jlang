#include <stdio.h>
#include "jlang_error.h"

void err_unexpected_character(int line, int column, char character)
{
          printf("[ERROR:%i:%i] Unexpected character '%c'\n", line, column, character);
}

void err_eof_in_string(int line, int column)
{

      printf("[ERROR:%i:%i] string did not terminate\n", line, column);
}
