#include <stdio.h>
#include "scan.h"
#include "jlang_parser/jlang_parser.h"
#include "jlang_interpreter/jlang_interpreter.h"
#include "jlang_source_code.h"
#include "jlang_token/jlang_token_list.h"
#include "jlang_program.h"
#include "vm.h"

void execute(char *file)
{
  jl_source_code_t *src = jl_source_code_from_file(file);
  jl_token_list_t *tokens = scan(src);
  jl_program_t *program = parse(tokens);
  vm_t* vm = vm_new();
  vm_push_frame(vm, stack_new());
  interprete(program, vm);
}
