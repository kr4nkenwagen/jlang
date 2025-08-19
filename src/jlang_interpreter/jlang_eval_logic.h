#include "../jlang_syntax/jlang_syntax.h"
#include "../vm.h"
#include "../jlang_program.h"

void eval_while(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_for(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_if(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_return(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_break(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_continue(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_error(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_out(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);

