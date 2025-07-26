#include <stdio.h>
#include "jlang_source_code.h"
#include "jlang_object/jlang_object.h"
#include "jlang_token/jlang_token.h"
#include "jlang_program.h"
#include "jlang_common.h"
#include "scan.h"
#include "jlang_parser/jlang_parser.h"
#include "repl.h"

void main ()
{
 // l_token_list_t *tokens = scan(jl_source_code_from_file("test.jonx"));

  //jl_program_t *syntax = parse(tokens);
  repl();
}
