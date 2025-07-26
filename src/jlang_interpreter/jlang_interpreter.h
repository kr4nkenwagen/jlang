#include "../jlang_object/jlang_object.h"
#include "../jlang_program.h"
#include "../vm.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object.h"

void interprete_branch(jl_syntax_t *syntax, vm_t *vm);
void eval_if(jl_syntax_t *syntax, vm_t *vm);
jl_object_t *interprete(jl_program_t *program, vm_t *vm);
