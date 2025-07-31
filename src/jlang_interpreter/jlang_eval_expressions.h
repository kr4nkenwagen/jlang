#include "../jlang_object/jlang_object.h"
#include "../jlang_program.h"
#include "../vm.h"
#include "../jlang_syntax/jlang_syntax.h"
#include "../jlang_object/jlang_object.h"
#include "../jlang_program.h"

jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_number(jl_syntax_t *syntax);
jl_object_t *eval_string_operation_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_unary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_comparison_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
bool divide_by_zero(jl_object_t *a, jl_object_t *b);
jl_object_t *eval_binary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
void eval_assignment_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
jl_object_t *eval_primary_expression(jl_syntax_t *syntax, vm_t *vm, jl_program_t *program);
