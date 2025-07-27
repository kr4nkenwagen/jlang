#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <string.h>
#include "jlang_object/jlang_object.h"
#include "jlang_common.h"

void jl_print_object(jl_object_t *target)
{
  if(target == NULL)
  {
    return;
  }
  switch (target->type) {  
    case NIL_OBJECT:
      printf("Object: NULL");
    break;
    case INT:
     printf("Object: %i\n", target->data.v_int);
    break;
    case FLOAT:
     printf("Object: %d\n", target->data.v_float);
    break;
    case STRING_OBJECT:
     printf("Object: %s\n", target->data.v_string);
    break;
    case BOOLEAN:
      if(target->data.v_bool == true)
      {
        printf("Object: true\n", target->data.v_string);
        break;
      }
      printf("Object: false\n", target->data.v_string);
    break;
    case ARRAY:
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
