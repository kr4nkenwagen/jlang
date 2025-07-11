#include <stdio.h>
#include "jlang_object.h"
#include "jlang_token.h"
#include "scan.h"
#include "parse.h"

void main ()
{
  jl_object_t *str = jl_new_string("1337");
  jl_object_t *num = jl_new_int(1337);
  jl_object_t *flo = jl_new_float(13.37);

  jl_object_t *numflo = jl_add(num, flo);
  jl_object_t *flonum = jl_add(flo, num);
  jl_object_t *floflo = jl_add(flo, flo);
  jl_object_t *numnum = jl_add(num, num);
  jl_object_t *strstr = jl_add(str, str);
  
  jl_token_list_t * tokens = scan("test.jonx");

  tokens = parse(tokens);
}
