#ifndef SLEX_H
#define SLEX_H

typedef enum {
  SLEX_TOK_eof,
  SLEX_TOK_str_lit,
  SLEX_TOK_char_lit,
  SLEX_TOK_int_lit,
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
  int parsed_int_lit;

} SlexContext;

// Description:
// - This function initializes the SlexContext struct.
// Parameters:
// - context: The struct to be initialized.
// - stream_start: Pointer to the first character in the stream.
// - stream_end: Pointer to the character just past the last character in the stream (or to EOF).
// - string_store: Pointer to the storage used for parsing strings.
// - string_store_len: Specifies the length of string_store.
void slex_init_context(SlexContext *context, char *stream_start,
                       char *stream_end, char* string_store, int string_store_len);

// Description:
// - This function parses a token and advances context->parse_ptr.
// Parameters:
// - context: The context needed for tokenizing.
// Returns:
// - Returns 1 if a token was parsed successfully; otherwise, returns a non-zero value.
int slex_get_next_token(SlexContext *context);

// Description:
// - This function retrieves the location of the last token.
// Parameters:
// - context: The parsing context.
// - stream_begin: Pointer to the location from where the lines and columns are counted.
// - line_num: Output pointer for the line number.
// - col_num: Output pointer for the column number.
void slex_get_token_location(SlexContext *context, char *stream_begin, 
                             int *line_num, int *col_num);

// Description:
// - This function returns the current location of the parsing point.
//   In the event of an error, the parsing point is updated to indicate the error's location.
//   Therefore, this function can also be used to identify the location of any errors that occur.
// Parameters:
// - context: The parsing context.
// - stream_begin: Pointer to the location from where the lines and columns are counted.
// - line_num: Output pointer for the line number.
// - col_num: Output pointer for the column number.
void slex_get_parse_ptr_location(SlexContext *context, char *stream_begin, 
                             int *line_num, int *col_num);

#ifdef SLEX_IMPLEMENTATION

/* -- Configuration -- */

// Whether to return `SLEX_TOK_eof` when at the end.
#ifndef SLEX_END_IS_TOKEN
#define SLEX_END_IS_TOKEN 0
#endif

/* -- Helper Functions -- */

static int slex_is_numeric(char c) {
  return c >= '0' && c <= '9';
}

static int slex_is_oct(char c) {
  return c >= '0' && c <= '7';
}

static int slex_is_hex(char c) {
  return slex_is_numeric(c) || (c >= 'A' && c <= 'F') 
         || (c >= 'a' && c <= 'f');
}

static int slex_hex_to_int(char c) {
  if(slex_is_numeric(c))
    return c - '0';
  if(c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if(c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  return -1;
}

static int slex_is_whitespace(char c) {
  return c == ' ' || c == '\t' ||
         c == '\n' || c == '\v' || 
         c == '\f' || c == '\r';
}

static int slex_get_end(SlexContext *ctx) {
#if SLEX_END_IS_TOKEN
    ctx->tok_ty = SLEX_TOK_eof;
    ctx->first_tok_char = ctx->parse_end;
    ctx->last_tok_char = ctx->parse_end;
    return 1;
#else
    return 0;
#endif
}

/* -- Implementation -- */

static int slex_parse_num(SlexContext *ctx) {
  char *end = ctx->parse_ptr;
  int fact = -1;
  while(end < ctx->parse_end) {
    if(!slex_is_numeric(*end))
      break;
    end++;
    if(fact != -1)
      fact *= 10;
    else
      fact = 1;
  }

  int len = end - ctx->parse_ptr;
  int num = 0;

  for(char *it = ctx->parse_ptr; it < end; it++) {
    num += (*it - '0') * fact;
    fact /= 10;
  }

  ctx->parsed_int_lit = num;
  ctx->parse_ptr = end;
  ctx->last_tok_char = end - 1;
  return 1;
}

static int slex_parse_int_lit(SlexContext *ctx) {
  ctx->first_tok_char = ctx->parse_ptr;
  ctx->tok_ty = SLEX_TOK_int_lit;

  if(*ctx->parse_ptr != '0')
    return slex_parse_num(ctx);

  // the current char is the last one
  if(ctx->parse_ptr >= ctx->parse_end - 1)
    goto zero;

  // hexadecimals
  if(ctx->parse_ptr[1] == 'x' || ctx->parse_ptr[1] == 'X') {
    ctx->parse_ptr += 2; // consume 0 and x

    if(ctx->parse_ptr >= ctx->parse_end)
      return 0;

    char *end = ctx->parse_ptr;
    int fact = -1;

    while(end < ctx->parse_end) {
      if(!slex_is_hex(*end))
        break;
      end++;
      if(fact != -1)
        fact *= 16;
      else
        fact = 1;
    }

    int len = end - ctx->parse_ptr;
    if(!len)
      return 0;

    int hex = 0;

    for(char *it = ctx->parse_ptr; it < end; it++) {
      hex += slex_hex_to_int(*it) * fact;
      fact /= 16;
    }

    ctx->parsed_int_lit = hex;
    ctx->parse_ptr = end;
    ctx->last_tok_char = end - 1;
    return 1;
  }

  // octals
  if(slex_is_numeric(ctx->parse_ptr[1])) {
    char *end = ctx->parse_ptr;
    int fact = -1;

    while(end < ctx->parse_end) {
      if(!slex_is_oct(*end))
        break;
      end++;
      if(fact != -1)
        fact *= 8;
      else
        fact = 1;
    }

    int len = end - ctx->parse_ptr;
    int oct = 0;

    for(char *it = ctx->parse_ptr; it < end; it++) {
      oct += (*it - '0') * fact;
      fact /= 8;
    }

    ctx->parsed_int_lit = oct;
    ctx->parse_ptr = end;
    ctx->last_tok_char = end - 1;
    return 1;
  }

zero:
  ctx->parsed_int_lit = 0;
  ctx->last_tok_char = ctx->first_tok_char;
  ctx->parse_ptr++;
  return 1;
}

static int slex_parse_esc_seq(SlexContext *ctx) {
  /* TODO: Add support for unicode escape sequences and allow 
           octs to be larger than 255. */

  ctx->parse_ptr++; // skip \

  if(ctx->parse_ptr >= ctx->parse_end)
    return -1;

  if(ctx->parse_ptr > ctx->parse_end - 3)
    goto other_sequence;

  if(slex_is_oct(*ctx->parse_ptr) && slex_is_oct(ctx->parse_ptr[1]) 
     && slex_is_oct(ctx->parse_ptr[2])) {
    int oct = 0;
    int fact = 64;
    for(char *it = ctx->parse_ptr; it < ctx->parse_ptr + 3; it++) {
      oct += (*it - '0') * fact;
      fact /= 8; 
      if(oct > 255)
        return -1;
    }
    ctx->parse_ptr += 3;
    return oct;
  }

  if(*ctx->parse_ptr == 'x' && slex_is_hex(ctx->parse_ptr[1]) 
     && slex_is_hex(ctx->parse_ptr[2])) {
    int hex = slex_hex_to_int(ctx->parse_ptr[1]) * 16 + slex_hex_to_int(ctx->parse_ptr[2]); 
    ctx->parse_ptr += 3;
    return hex;
  }

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

static int slex_parse_char_lit(SlexContext *ctx) {
  ctx->first_tok_char = ctx->parse_ptr;
  ctx->str_len = 1;
  ctx->tok_ty = SLEX_TOK_char_lit;
  ctx->parse_ptr++; // skip '
  
  if(ctx->parse_ptr >= ctx->parse_end)
    return 0;

  if(*ctx->parse_ptr == '\\') {
    int seq = slex_parse_esc_seq(ctx);

    if(seq == -1)
      return 0;
    
    *ctx->string_store = seq;
  }
  else if(*ctx->parse_ptr == '\'')
    return 0; // empty char lit not allowed
  else {
    *ctx->string_store = *ctx->parse_ptr;
    ctx->parse_ptr++;
  }

  if(ctx->parse_ptr >= ctx->parse_end || *ctx->parse_ptr != '\'')
    return 0;

  ctx->last_tok_char = ctx->parse_ptr;
  ctx->parse_ptr++;
  
  return 1;
}

static int slex_parse_str_lit(SlexContext *ctx) {
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
      int c = slex_parse_esc_seq(ctx);
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
  if(ctx->parse_ptr >= ctx->parse_end) 
    return slex_get_end(ctx);
  
  while(ctx->parse_ptr <= ctx->parse_end - 1) {
   if(!slex_is_whitespace(*ctx->parse_ptr)) 
     break;
   ctx->parse_ptr++;
  }

  if(ctx->parse_ptr >= ctx->parse_end) 
    return slex_get_end(ctx);

  if(slex_is_numeric(*ctx->parse_ptr))
    return slex_parse_int_lit(ctx);
  
  switch(*ctx->parse_ptr) {
    default: return 0;
    case '"': return slex_parse_str_lit(ctx);
    case '\'': return slex_parse_char_lit(ctx);
  }
}

void slex_get_token_location(SlexContext *ctx, char *stream_begin, int *line_num, int *col_num) {
  int ln = 1;
  int col = 0;

  for(char *it = stream_begin; it < ctx->first_tok_char; it++) {
    if(*it == '\n') {
      ln++;
      col = 1;
    }
    else {
      col++;
    }
  }

  *line_num = ln;
  *col_num = col + 1;
}

void slex_get_parse_ptr_location(SlexContext *ctx, char *stream_begin, int *line_num, int *col_num) {
  int ln = 1;
  int col = 0;

  for(char *it = stream_begin; it < ctx->parse_ptr; it++) {
    if(*it == '\n') {
      ln++;
      col = 1;
    }
    else {
      col++;
    }
  }

  *line_num = ln;
  *col_num = col + 1;
}
#endif // SLEX_IMPLEMENTATION
#endif // SLEX_H
