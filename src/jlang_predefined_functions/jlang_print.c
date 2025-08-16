#include <stdio.h>
#include "../jlang_object/jlang_object_string.h"
#include "../jlang_object/jlang_object.h"

void print_out(char *str)
{
  for(int i = 0; str[i] != '\0'; i++)
  {
    if(str[i] == '\\' && str[i+1] != '\0')
    {
      switch(str[++i])
      {
        case 'n':
          printf("%c", 10);
        continue;
        case 't':
          printf("%c", 9);
        continue;
        default:
          i--;
      }
    }
    putchar(str[i]);
  }
}

void jl_print(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  switch(obj->type)
  {
    case STRING_OBJECT:
      print_out(obj->data.v_string);
    break;
    case INT_OBJECT:
      print_out(int_to_number(obj->data.v_int));
    break;
  }
}
