#include "jlang_object.h"

jl_object_t *jl_substring(jl_object_t *obj, int start, int length);
int jl_position_of_first_instance(jl_object_t *obj, char *instance);
int jl_position_of_last_instance(jl_object_t *obj, char *instance);
char *join_string(char *a, char *b);
char *int_to_number(int num);

void jl_shorten_string(jl_object_t *target, jl_object_t *length);
int jl_string_length(jl_object_t *target);
void jl_strip_instances_from_string(jl_object_t *target, jl_object_t *instance);
void jl_multiply_string(jl_object_t *target, jl_object_t *multiplier);
void jl_divide_string(jl_object_t *target, jl_object_t *divide);
void jl_modulus_string(jl_object_t *target, jl_object_t *modulus);
