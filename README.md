# slex

A simple C lexer written in C inspired from [stb_c_lexer](https://github.com/nothings/stb/blob/master/stb_c_lexer.h)

## Features

- Only one header file.
- No standard lib.
- No crashes on ill formed input.

## Quickstart

```c
#include <stdio.h>
#include <stdlib.h>

#define SLEX_IMPLEMENTATION
#include "slex.h"

int main(int argc, char** argv) {
  FILE *f = fopen("file.test", "rb");
  char *text = (char *)malloc(1 << 20);
  int len = f ? (int)fread(text, 1, 1 << 20, f) : -1;

  if (len < 0) {
    fprintf(stderr, "Error opening file\n");
    free(text);
    fclose(f);
    return 1;
  }

  fclose(f);

  SlexContext ctx;
  char store[1024];

  slex_init_context(&ctx, text, text + len, store, 1024);

  while(slex_get_next_token(&ctx)){
    // ...
  }
}
```
