#ifndef SLEX_H
#define SLEX_H

#define SLEX_IMPLEMENTATION

typedef enum {
  SLEX_TOK_eof,
  SLEX_TOK_str_lit,
} TokenType;

typedef struct {
  char *parse_ptr;
  char *parse_end;
  char *string_store;
  int string_store_len;

  TokenType tok_ty;
  char *first_tok_char;
  char *last_tok_char;
  int str_len;

} SlexContext;

// This function initilizes the `SlexContext` struct.
// Params:
//  - `context` is the struct to be initilized.
//  - `stream_start` points to the first char in the stream.
//  - `stream_end` points to the last char of the stream + 1 (or to EOF).
//  - `string_store` points to the storage used for parsing strings.
//  - `string_store_len` specifies the length of `string_store`.
void slex_init_context(SlexContext *context, char *stream_start,
                       char *stream_end, char* string_store, int string_store_len);

// This functions parses a token and advances `context->parse_ptr`.
// Params:
//  - `context` is the context needed for tokenizing.
// Return:
//  Returns 1 if a token was parsed without an error, otherwise non-zero.
int slex_get_next_token(SlexContext *context);

#ifdef SLEX_IMPLEMENTATION

/* -- Configuration -- */

// Whether to return `SLEX_TOK_eof` when at the end.
#ifndef SLEX_END_IS_TOKEN
#define SLEX_END_IS_TOKEN 0
#endif

/* -- Implementation -- */

static int slex_is_oct(char c) {
  return c >= '0' && c <= '7';
}

static int slex_is_hex(char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

static int slex_hex_to_int(char c) {
  if(c >= '0' && c <= '9')
    return c - '0';
  if(c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if(c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  return -1;
}

static int slex_parse_escape_seq(SlexContext *ctx) {
  ctx->parse_ptr++; // skip \

  if(ctx->parse_ptr >= ctx->parse_end)
    return -1;

  if(ctx->parse_ptr > ctx->parse_end - 3)
    goto other_sequence;

  // FIX: Some octals overflow 8 bits
  if(slex_is_oct(*ctx->parse_ptr) && slex_is_oct(ctx->parse_ptr[1]) && slex_is_oct(ctx->parse_ptr[2])) {
    int oct = (*ctx->parse_ptr - '0') * 64 + (ctx->parse_ptr[1] - '0') * 8 +
      (ctx->parse_ptr[2] - '0');
    ctx->parse_ptr += 3;
    return oct;
  }
  
  if(*ctx->parse_ptr == 'x' && slex_is_hex(ctx->parse_ptr[1]) && slex_is_hex(ctx->parse_ptr[2])) {
    int hex = slex_hex_to_int(ctx->parse_ptr[1]) * 16 + slex_hex_to_int(ctx->parse_ptr[2]); 
    ctx->parse_ptr += 3;
    return hex;
  }

  // TODO: Unicode constants

other_sequence:
  ctx->parse_ptr++;
  switch(ctx->parse_ptr[-1]) {
    default: ctx->parse_ptr--; return -1;
    case 'a': return '\a';
    case 'b': return '\b';
    case 'e': return '\e';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    case '\\': return '\\';
    case '\'': return '\'';
    case '"': return '"';
    case '?': return '\?';
    case '0': return '\0';
  }
}

static int slex_parse_string(SlexContext *ctx) {
  int curr_str_len = 0;

  ctx->tok_ty = SLEX_TOK_str_lit;
  ctx->first_tok_char = ctx->parse_ptr;

  ctx->parse_ptr++; // skip "
  
  while(ctx->parse_ptr < ctx->parse_end) {

    if(*ctx->parse_ptr == '"') {
      ctx->last_tok_char = ctx->parse_ptr;
      ctx->str_len = curr_str_len;
      ctx->parse_ptr++;
      return 1;
    }

    if(curr_str_len > ctx->string_store_len)
      return 0;
    
    if(*ctx->parse_ptr == '\\') {
      int c = slex_parse_escape_seq(ctx);
      if(c == -1)
        return 0;
      ctx->string_store[curr_str_len] = c;
    }
    else {
      ctx->string_store[curr_str_len] = *ctx->parse_ptr;
      ctx->parse_ptr++;
    }
    curr_str_len++;
  }

  return 0;
}

void slex_init_context(SlexContext *ctx, char *stream_start,
                       char *stream_end, char* string_store, int string_store_len) {
  ctx->parse_ptr = stream_start;
  ctx->parse_end = stream_end;
  ctx->string_store = string_store;
  ctx->string_store_len = string_store_len;
}

int slex_get_next_token(SlexContext *ctx) {
  if(ctx->parse_ptr >= ctx->parse_end) {
#if SLEX_END_IS_TOKEN
    ctx->tok_ty = SLEX_TOK_eof;
    ctx->first_tok_char = ctx->parse_end;
    ctx->last_tok_char = ctx->parse_end;
    return 1;
#else
    return 0;
#endif
  }
  
  switch(*ctx->parse_ptr) {
    default: return 0;
    case '"': return slex_parse_string(ctx);
  }
}

#endif // SLEX_IMPLEMENTATION
#endif // SLEX_H
