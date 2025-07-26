#include <stdlib.h>

typedef enum jl_object_type {
  INT,
  FLOAT,
  STRING,
  ARRAY,
  VECTOR,
  NIL_OBJECT,
  BOOLEAN,
  FUNCTION_OBJECT
}jl_object_type_t;

typedef struct jl_object jl_object_t;

typedef struct jl_array {
  size_t size;
  int count;
  jl_object_t **elements;
}jl_array_t;

typedef struct jl_vector {
  jl_object_t *x;
  jl_object_t *y;
  jl_object_t *z;
}jl_vector_t;

typedef union jl_object_data { 
  int v_int;
  float v_float;
  char *v_string;
  bool v_bool;
  void *v_funct;
  jl_vector_t *v_vector;
  jl_array_t *v_array;
}jl_object_data_t;

typedef struct jl_object {
  bool is_marked;
  bool is_const;
  int refcount;
  jl_object_type_t type;
  char *name;
  jl_object_data_t data;
}jl_object_t;

jl_object_t *jl_new_int(int value);
jl_object_t *jl_new_float(float value);
jl_object_t *jl_new_string(char *value);
jl_object_t *jl_new_array();
jl_object_t *jl_new_null();
jl_object_t *jl_new_bool(bool value);
jl_object_t *jl_new_vector(jl_object_t *x, jl_object_t *y, jl_object_t *z);
jl_object_t *jl_new_funct(void *syntax);

void jl_object_free(jl_object_t *obj);

jl_object_t *jl_substring(jl_object_t *obj, int start, int length);
size_t jl_length(jl_object_t * obj);
int jl_position_of_first_instance(jl_object_t *obj, char *instance);
int jl_position_of_last_instance(jl_object_t *obj, char *instance);

jl_object_t *jl_add(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_subtract(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_multiply(jl_object_t *target, jl_object_t *multiplier);
jl_object_t *jl_divide(jl_object_t *target, jl_object_t *divider);
jl_object_t *jl_modulus(jl_object_t *target, jl_object_t *modulus);
void jl_assign(jl_object_t *target, jl_object_t *source);
void jl_array_set(jl_object_t *array, int index, jl_object_t *obj);
jl_object_t *jl_array_get(jl_object_t *array, int index); 
void jl_object_set_null(jl_object_t *obj);

jl_object_t *jl_equals(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_not_equals(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_greater_equals(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_greater(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_less(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_less_equals(jl_object_t *a, jl_object_t *b);

void ref_free(jl_object_t *obj); 
void ref_dec(jl_object_t *obj);
