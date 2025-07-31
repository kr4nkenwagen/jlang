#include "../jlang_syntax/jlang_syntax.h"
#include "../vm.h"
#include "../jlang_program.h"

void eval_while(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_for(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_if(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);

