#include <stdio.h>
#include <stdlib.h>

#define SLEX_IMPLEMENTATION
#define SLEX_END_IS_TOKEN 1
#include "../src/slex.h"

int main(int argc, char **argv) {

  SlexContext ctx;
  char store[1024];

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

  slex_init_context(&ctx, text, text + len, store, 1024);
  for(;;){
    if(!slex_get_next_token(&ctx)) {
      printf("Invalid syntax at '%c'\n", *ctx.parse_ptr);
      ctx.parse_ptr++;
      continue;
    }

    if(ctx.tok_ty == SLEX_TOK_eof) {
      break;
    }

    int len = ctx.last_tok_char - ctx.first_tok_char + 1;
    printf("%.*s\n", len, ctx.first_tok_char);

    if(ctx.tok_ty == SLEX_TOK_str_lit)
      printf("Parsed string: %.*s\n", ctx.str_len, ctx.string_store);

    getc(stdin);
  }
}
