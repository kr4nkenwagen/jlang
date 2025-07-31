#include "../jlang_object/jlang_object.h"
#include "../jlang_program.h"
#include "../vm.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object.h"
#include "../jlang_program.h"

void eval_variable_declarations(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_array_declaration(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_array_identifier(jl_syntax_t *syntax, vm_t *vm, jl_object_t *obj, jl_program_t *program);
jl_object_t *eval_identifier(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
