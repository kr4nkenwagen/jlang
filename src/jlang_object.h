#include <stdlib.h>

typedef enum jl_object_type {
  INT,
  FLOAT,
  STRING,
  ARRAY,
  NIL_OBJECT
}jl_object_type_t;

typedef struct jl_object jl_object_t;

typedef struct jl_array {
  size_t size;
  jl_object_t **elements;
}jl_array_t;

typedef union jl_object_data {
  int v_int;
  float v_float;
  char *v_string;
  jl_array_t *v_array;
}jl_object_data_t;

typedef struct jl_object {
  bool is_marked;
  int refcount;
  jl_object_type_t type;
  jl_object_data_t data;
}jl_object_t;

jl_object_t *jl_new_int(int value);
jl_object_t *jl_new_float(float value);
jl_object_t *jl_new_string(char *value);
jl_object_t *jl_new_array(size_t size);
jl_object_t *jl_new_null();

void jl_object_free(jl_object_t *obj);

size_t jl_length(jl_object_t * obj);
jl_object_t *jl_add(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_subtract(jl_object_t *a, jl_object_t *b);
jl_object_t *jl_multiply(jl_object_t *target, jl_object_t *multiplimultiplier);
void jl_array_set(jl_object_t *array, size_t index, jl_object_t *obj);
jl_object_t * jl_array_get(jl_object_t *array, size_t index); 
void jl_object_set_null(jl_object_t *obj);

void ref_free(jl_object_t *obj); 
void ref_dec(jl_object_t *obj);
