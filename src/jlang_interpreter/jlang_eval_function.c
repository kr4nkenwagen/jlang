#include "jlang_eval_function.h"
#include "jlang_interpreter.h"
#include "../vm.h"
#include "../stack.h"
#include "../jlang_token/jlang_token.h"
#include "../jlang_syntax/jlang_syntax.h"

jl_object_t *eval_function_identifier(jl_syntax_t *syntax, vm_t *vm)
{
  interprete_branch(syntax, vm);
}

void eval_function_declaration(jl_syntax_t *syntax, vm_t *vm)
{
  jl_object_t *funct = jl_new_funct(syntax->right);
  funct->name = syntax->right->token->literal;
  stack_push(vm_curr_frame(vm), funct);
}


