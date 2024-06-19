# slex

A simple C lexer written in C inspired from [stb_c_lexer](https://github.com/nothings/stb/blob/master/stb_c_lexer.h).
Note: "slex" stands for "safe lexer" because it should not crash on any input.

## Features

- Only one header file.
- No standard lib.
- No crashes on ill formed input.

## Quickstart

```c
#include <stdio.h>
#include <stdlib.h>

// this macro must be defined in one file(translation unit) in order to avoid ODR violations.
#define SLEX_IMPLEMENTATION
#include "slex.h"

int main(int argc, char** argv) {
  // open and read c file 
  FILE *f = fopen("sample.c", "rb");
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

  // initilize lexer
  slex_init_context(&ctx, text, text + len, store, 1024);

  while(slex_get_next_token(&ctx)){
    // do stuff with tokens
  }
}
```
