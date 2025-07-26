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

size_t jl_length(jl_object_t * obj);
void jl_object_set_null(jl_object_t *obj);

void ref_free(jl_object_t *obj); 
void ref_dec(jl_object_t *obj);
