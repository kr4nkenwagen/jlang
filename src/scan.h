#include "jlang_token.h"

typedef struct source_code {
  int is_at_end;
  size_t size;
  int pointer;
  int line;
  int column;
  char *src;
}source_code_t;

source_code_t *open_src(char *src);
source_code_t *from_repl_line(char *line);
char advance(source_code_t *src);
char peek(source_code_t *src, int distance);
void clear_buffer(source_code_t *src);
jl_token_list_t *scan(source_code_t *src);

