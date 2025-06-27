#include <stdlib.h>

typedef struct source_code {
  char *src;
  size_t size;
  int pointer;
  bool is_at_end;
}source_code_t;

source_code_t *open_src(char *src);
char advance(source_code_t *src);
char peek(source_code_t *src, int distance);

void scan(char *src);

