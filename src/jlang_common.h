#include "jlang_object.h"
#include <stdlib.h>

int jl_string_length(jl_object_t *target);
void jl_strip_instances_from_string(jl_object_t *target, jl_object_t *instance);
void jl_shorten_string(jl_object_t *target, jl_object_t *length);
void jl_multiply_string(jl_object_t *target, jl_object_t *multiplier);
void jl_divide_string(jl_object_t *target, jl_object_t *divide);
void jl_modulus_string(jl_object_t *target, jl_object_t *modulus);
void **jl_double_array_size(void *arr, size_t curr_size);

void jl_print_object(jl_object_t *target);
