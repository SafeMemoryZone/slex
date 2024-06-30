#include <stdio.h>
#include <stdlib.h>
#define SLEX_IMPLEMENTATION
#include "../src/slex.h"

#define TESTFILE "sample.c"

int main(int argc, char **argv) {
  SlexContext ctx;
  char store[1024];

  FILE *f = fopen(TESTFILE, "rb");
  char *text = (char *)malloc(1 << 20);
  int len = f ? (int)fread(text, 1, 1 << 20, f) : -1;

  if (len < 0) {
    fprintf(stderr, "Error opening file\n");
    free(text);
    fclose(f);
    return 1;
  }

  fclose(f);

  slex_init_context(&ctx, text, text + len, store, 1024);
  for (;;) {
    if (!slex_get_next_token(&ctx)) {
      int ln;
      int col;
      slex_get_parse_ptr_location(&ctx, text, &ln, &col);
      printf("- An error occured at %d:%d\n", ln, col);
      ctx.parse_point++;
      continue;
    }

    if (ctx.tok_ty == SLEX_TOK_eof)
      break;

    int len = ctx.last_tok_char - ctx.first_tok_char + 1;
    printf("+ Parsed token: %.*s\n", len, ctx.first_tok_char);

    if (ctx.tok_ty == SLEX_TOK_str_lit || ctx.tok_ty == SLEX_TOK_char_lit)
      printf("    Extracted string or char: %.*s\n", ctx.str_len,
             ctx.string_store);

    else if (ctx.tok_ty == SLEX_TOK_int_lit)
      printf("    Extracted int literal: %llu\n", ctx.parsed_int_lit);

    else if (ctx.tok_ty == SLEX_TOK_float_lit)
      printf("    Extracted float literal: %f\n", ctx.parsed_float_lit);
  }
}
