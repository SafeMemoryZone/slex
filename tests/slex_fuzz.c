#include <stdio.h>
#define SLEX_IMPLEMENTATION
#include "../src/slex.h"

#include <stdint.h>
#include <stddef.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  SlexContext ctx;
  char store[1024];

  slex_init_context(&ctx, (char*)Data, (char*)Data + Size, store, 1024);
  for (;;) {
    if (!slex_get_next_token(&ctx)) {
      int ln;
      int col;
      slex_get_parse_ptr_location(&ctx, (char*)Data, &ln, &col);
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
  }
  return 0;
}
