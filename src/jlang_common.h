#include "jlang_object.h"

int jl_string_length(jl_object_t *target);
void jl_strip_instances_from_string(jl_object_t *target, jl_object_t *instance);
void jl_shorten_string(jl_object_t *target, jl_object_t *length);
void jl_multiply_string(jl_object_t *target, jl_object_t *multiplier);


void jl_print_object(jl_object_t *target);
