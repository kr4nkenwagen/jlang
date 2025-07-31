#include <stdio.h>
#include "jlang_print.h"
#include "../jlang_object/jlang_object.h"
#include "../jlang_object/jlang_object_operations.h"


void jl_println(jl_object_t *obj)
{
  if(obj == NULL)
  {
    return;
  }
  jl_object_t *newline = jl_new_string("\n");
  jl_print(jl_add(obj, newline));
}
