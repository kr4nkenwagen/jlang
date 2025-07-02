#include <stdlib.h>
#include "jlang_token.h"

typedef struct source_code {
  bool is_at_end;
  size_t size;
  int pointer;
  int line;
  int column;
  char *buffer;
  char *src;
}source_code_t;

source_code_t *open_src(char *src);
char advance(source_code_t *src);
char peek(source_code_t *src, int distance);
void clear_buffer(source_code_t *src);
jl_token_list_t *scan(char *src);

