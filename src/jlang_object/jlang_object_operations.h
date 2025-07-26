#include "jlang_object.h"

jl_object_t *jl_add(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_subtract(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_multiply(jl_object_t *target, jl_object_t *multiplier);
jl_object_t *jl_divide(jl_object_t *target, jl_object_t *divider);
jl_object_t *jl_modulus(jl_object_t *target, jl_object_t *modulus);
void jl_assign(jl_object_t *target, jl_object_t *source);

