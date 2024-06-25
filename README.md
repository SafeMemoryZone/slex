# slex

A simple C lexer written in C, inspired by [stb_c_lexer](https://github.com/nothings/stb/blob/master/stb_c_lexer.h).

### Note

"slex" stands for "safe lexer" because it is designed to handle any input without crashing.

## Features

- Single header file for easy integration.
- Simple and user-friendly API.
- No dependencies on the standard library.
- Robust handling of malformed input to prevent crashes.

## Example/Quickstart

```c
#include <stdio.h>
#include <stdlib.h>

/* This macro must be defined in one file (translation unit)
   to avoid ODR (One Definition Rule) violations.
*/
#define SLEX_IMPLEMENTATION
#include "slex.h"

int main(int argc, char** argv) {
  // Open and read a sample C file 
  FILE *f = fopen("sample.c", "rb");
  char *text = (char *)malloc(1 << 20);  // Allocate 1 MB for the file content
  int len = f ? (int)fread(text, 1, 1 << 20, f) : -1;

  if (len < 0) {
    fprintf(stderr, "Error opening file\n");
    free(text);
    if (f) fclose(f);
    return 1;
  }

  fclose(f);

  /* Fields you'll use are:
     ctx.tok_ty: holds the parsed token type
     ctx.first_tok_char: pointer to the first character in a token
     ctx.last_tok_char: pointer to the last character in a token
     ctx.str_len: if a string or character literal was parsed, it holds the length in bytes
     ctx.parsed_int_lit: if an integer literal was parsed, it holds the parsed integer value (excluding the '-')
  */
  SlexContext ctx;

  // Storage used by the lexer to store parsed strings and characters (recommended to be 1024+ bytes)
  // NOTE: Every string or character literal is UTF-8 encoded
  char store[1024];

  // Initialize the lexer context
  slex_init_context(&ctx, text, text + len, store, 1024);

  // The lexer only depends on ctx.parse_point and ctx.stream_end.

  /* slex_get_next_token returns 1 if a token was parsed, otherwise 0.
     When an error occurs, ctx.tok_ty indicates the kind of error,
     and ctx.parse_point points to the location of the error, while
     ctx.first_tok_char points to where it began parsing.
     You can then use slex_get_parse_ptr_location to get the error location.
  */
  while (slex_get_next_token(&ctx)) {
    // Process tokens here
  }

  free(text);
  return 0;
}
```

## License

This project is licensed under the MIT License. 
