#!/bin/sh
gcc src/scan.c src/jlang_token.c src/jlang_error.c tests/*.c -lcunit -I src/ && ./a.out
rm a.out
