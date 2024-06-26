#ifndef SLEX_H
#define SLEX_H

/* -- Configuration -- */

// Whether to return SLEX_TOK_eof when at the end.
#ifndef SLEX_END_IS_TOKEN
#define SLEX_END_IS_TOKEN 1
#endif

// Whether to add support for some CXX specific tokens.
#ifndef SLEX_ADD_CXX_SUPPORT
#define SLEX_ADD_CXX_SUPPORT 1
#endif

// Whether to parse int suffixes as a part of the token.
#ifndef SLEX_PARSE_INT_SUFFIXES
#define SLEX_PARSE_INT_SUFFIXES 1
#endif

// Whether to skip the current line when # is encountered.
#ifndef SLEX_SKIP_PREPROCESSOR
#define SLEX_SKIP_PREPROCESSOR 0
#endif

typedef enum {
  SLEX_ERR_unknown_tok,      // Token unrecognised
  SLEX_ERR_parse,            // Token recognised, but contains an error 
  SLEX_ERR_storage,          // Not enough storage for token (int literal to big, string larger than string store, ...)

  SLEX_TOK_eof,              // End of File (returned when SLEX_END_IS_TOKEN is enabled)
  SLEX_TOK_str_lit,          // String Literal ("hello, world\n", "abc\0", ...)
  SLEX_TOK_char_lit,         // Character Literal ('h', 'hello', '\x5f', ...)
  SLEX_TOK_int_lit,          // Integer Literal (0x12, 123, 030, 0b111, ...)
  SLEX_TOK_identifier,       // Identifier (foo, bar, ...)
  SLEX_TOK_l_square_paren,   // Left Square Bracket ([)
  SLEX_TOK_r_square_paren,   // Right Square Bracket (])
  SLEX_TOK_l_paren,          // Left Parenthesis (()
  SLEX_TOK_r_paren,          // Right Parenthesis ())
  SLEX_TOK_l_brace,          // Left Brace ({)
  SLEX_TOK_r_brace,          // Right Brace (})
  SLEX_TOK_period,           // Period (.)
  SLEX_TOK_ellipsis,         // Ellipsis (...)
  SLEX_TOK_bitwise_and,      // Bitwise AND (&)
  SLEX_TOK_and,              // Logical AND (&&)
  SLEX_TOK_bitwise_and_eq,   // Bitwise AND Assignment (&=)
  SLEX_TOK_mul,              // Multiplication (*)
  SLEX_TOK_mul_eq,           // Multiplication Assignment (*=)
  SLEX_TOK_plus,             // Plus (+)
  SLEX_TOK_inc,              // Increment (++)
  SLEX_TOK_plus_eq,          // Addition Assignment (+=)
  SLEX_TOK_minus,            // Minus (-)
  SLEX_TOK_arrow,            // Dereference Access (->)
  SLEX_TOK_dec,              // Decrement (--)
  SLEX_TOK_minus_eq,         // Subtraction Assignment (-=)
  SLEX_TOK_bitwise_not,      // Bitwise NOT (~)
  SLEX_TOK_not,              // Logical NOT (!)
  SLEX_TOK_not_eq,           // Not Equal (!=)
  SLEX_TOK_div,              // Division (/)
  SLEX_TOK_div_eq,           // Division Assignment (/=)
  SLEX_TOK_mod,              // Modulus (%)
  SLEX_TOK_mod_eq,           // Modulus Assignment (%=)
  SLEX_TOK_less,             // Less Than (<)
  SLEX_TOK_shl,              // Shift Left (<<)
  SLEX_TOK_less_or_eq,       // Less Than or Equal (<=)
  SLEX_TOK_shl_eq,           // Shift Left Assignment (<<=)
  SLEX_TOK_spaceship,        // Spaceship (<=>)
  SLEX_TOK_greater,          // Greater Than (>)
  SLEX_TOK_shr,              // Shift Right (>>)
  SLEX_TOK_greater_or_eq,    // Greater Than or Equal (>=)
  SLEX_TOK_shr_eq,           // Shift Right Assignment (>>=)
  SLEX_TOK_xor,              // Bitwise XOR (^)
  SLEX_TOK_xor_eq,           // Bitwise XOR Assignment (^=)
  SLEX_TOK_bitwise_or,       // Bitwise OR (|)
  SLEX_TOK_or,               // Logical OR (||)
  SLEX_TOK_bitwise_or_eq,    // Bitwise OR Assignment (|=)
  SLEX_TOK_questionmark,     // Question Mark (?)
  SLEX_TOK_colon,            // Colon (:)
  SLEX_TOK_semi,             // Semicolon (;)
  SLEX_TOK_assign,           // Assignment (=)
  SLEX_TOK_equality,         // Equality (==)
  SLEX_TOK_comma,            // Comma (,)
  SLEX_TOK_preprocessor,     // Preprocessor (#)
  SLEX_TOK_concat,           // Token Concatenation (##)
  SLEX_TOK_preprocessor_at,  // Preprocessor At (#@)
#if SLEX_ADD_CXX_SUPPORT
  SLEX_TOK_member_access,    // Member Access via Pointer to Member (.*)
  SLEX_TOK_deref_access,     // Dereference Access via Pointer to Member (->*)
  SLEX_TOK_scope_resolution, // Scope Resolution (::)
#endif
} TokenType;

typedef struct {
  char *parse_point;
  char *stream_end;
  char *string_store;
  int string_store_len;

  int tok_ty;
  char *first_tok_char;
  char *last_tok_char;
  int str_len;
  unsigned long long parsed_int_lit;

} SlexContext;

#ifdef __cplusplus
extern "C" {
#endif

// Description:
// - This function initializes the SlexContext struct.
// Parameters:
// - context: The struct to be initialized.
// - stream_start: Pointer to the first character in the stream.
// - stream_end: Pointer to the character just past the last character in the stream (or to EOF).
// - string_store: Pointer to the storage used for parsing strings.
// - string_store_len: Specifies the length of string_store.
void slex_init_context(SlexContext *context, char *stream_start, char *stream_end, char* string_store, int string_store_len);

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
void slex_get_token_location(SlexContext *context, char *stream_begin, int *line_num, int *col_num);

// Description:
// - This function returns the current location of the parsing point.
//   In the event of an error, the parsing point is updated to indicate the error's location.
//   Therefore, this function can also be used to identify the location of any errors that occur.
// Parameters:
// - context: The parsing context.
// - stream_begin: Pointer to the location from where the lines and columns are counted.
// - line_num: Output pointer for the line number.
// - col_num: Output pointer for the column number.
void slex_get_parse_ptr_location(SlexContext *context, char *stream_begin, int *line_num, int *col_num);

#ifdef __cplusplus
}
#endif

#ifdef SLEX_IMPLEMENTATION

static int slex_is_numeric(char c) {
  return c >= '0' && c <= '9';
}

static int slex_is_oct(char c) {
  return c >= '0' && c <= '7';
}

static int slex_is_hex(char c) {
  return slex_is_numeric(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

static int slex_hex_to_int(char c) {
  if(slex_is_numeric(c)) return c - '0';
  if(c >= 'A' && c <= 'F') return c - 'A' + 10;
  if(c >= 'a' && c <= 'f') return c - 'a' + 10;
  return -1;
}

static int slex_is_whitespace(char c) {
  return c == ' '  || c == '\t' || c == '\n' || c == '\v' || 
    c == '\f' || c == '\r';
}

static int slex_is_ident(char c) {
  return slex_is_numeric(c) || (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z') || c == '_';
}

static int slex_return_err(int err_ty, SlexContext *ctx) {
  ctx->last_tok_char = ctx->parse_point;
  ctx->tok_ty = err_ty;
  return 0;
}

static void slex_parse_int_suffix(SlexContext *ctx) {
  while(ctx->parse_point < ctx->stream_end) {
    char c = *ctx->parse_point;
    if(!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z'))
      break;
    ctx->parse_point++;
  }
  ctx->last_tok_char = ctx->parse_point - 1;
}

static int slex_return_eof(SlexContext *ctx) {
#if SLEX_END_IS_TOKEN
  ctx->tok_ty = SLEX_TOK_eof;
  ctx->first_tok_char = ctx->stream_end;
  ctx->last_tok_char = ctx->stream_end;
  return 1;
#else
  return slex_return_err(ctx);
#endif
}

static int slex_consume_single_char(SlexContext *ctx, TokenType ty)  {
  ctx->tok_ty = ty;
  ctx->first_tok_char = ctx->parse_point;
  ctx->last_tok_char = ctx->first_tok_char;
  ctx->parse_point++;
  return 1;
}

static int slex_try_match(SlexContext *ctx, TokenType match_ty, char *tok, int tok_len) {
  if (ctx->parse_point > ctx->stream_end - tok_len) 
    return 0;

  for (int i = 0; i < tok_len; i++) {
    if (ctx->parse_point[i] != tok[i]) {
      return 0;
    }
  }

  ctx->tok_ty = match_ty;
  ctx->first_tok_char = ctx->parse_point;
  ctx->last_tok_char = ctx->parse_point + tok_len - 1;
  ctx->parse_point += tok_len;
  return 1;
}

static int slex_mul_overflows_u64(unsigned long long a, unsigned long long b) {
  if(a == 0 || b == 0) return 0;
  return a > 0xFFFFFFFFFFFFFFFF / b;
}

static int slex_add_overflows_u64(unsigned long long a, unsigned long long b) {
  return a > 0xFFFFFFFFFFFFFFFF - b;
}

static int slex_utf8_encode_esc_seq(SlexContext *ctx, long long codepoint, char *loc) {
  if (codepoint > 0x10FFFF || loc >= ctx->string_store + ctx->string_store_len)
    return slex_return_err(SLEX_ERR_storage, ctx) -1;

  if (codepoint <= 0x7F) {
    *loc = codepoint;
    return 1;
  }
  else if (codepoint <= 0x7FF) {
    if (loc >= ctx->string_store + ctx->string_store_len - 1)
      return slex_return_err(SLEX_ERR_storage, ctx) -1;

    loc[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
    loc[1] = 0x80 | (codepoint & 0x3F);
    return 2;
  }
  else if (codepoint <= 0xFFFF) {
    if (loc >= ctx->string_store + ctx->string_store_len - 2)
      return slex_return_err(SLEX_ERR_storage, ctx) -1;

    loc[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
    loc[1] = 0x80 | ((codepoint >> 6) & 0x3F);
    loc[2] = 0x80 | (codepoint & 0x3F);
    return 3;
  }
  else {
    if (loc >= ctx->string_store + ctx->string_store_len - 3)
      return slex_return_err(SLEX_ERR_storage, ctx) -1;

    loc[0] = 0xF0 | ((codepoint >> 18) & 0x07);
    loc[1] = 0x80 | ((codepoint >> 12) & 0x3F);
    loc[2] = 0x80 | ((codepoint >> 6) & 0x3F);
    loc[3] = 0x80 | (codepoint & 0x3F);
    return 4;
  }
}

static int slex_parse_punctuator(SlexContext *ctx) {
  switch (*ctx->parse_point) {
    case '[':
      return slex_consume_single_char(ctx, SLEX_TOK_l_square_paren);
    case ']':
      return slex_consume_single_char(ctx, SLEX_TOK_r_square_paren);
    case '(':
      return slex_consume_single_char(ctx, SLEX_TOK_l_paren);
    case ')':
      return slex_consume_single_char(ctx, SLEX_TOK_r_paren);
    case '{':
      return slex_consume_single_char(ctx, SLEX_TOK_l_brace);
    case '}':
      return slex_consume_single_char(ctx, SLEX_TOK_r_brace);
    case '.':
      if (slex_try_match(ctx, SLEX_TOK_ellipsis, "...", 3)) return 1;
#if SLEX_ADD_CXX_SUPPORT
      if (slex_try_match(ctx, SLEX_TOK_member_access, ".*", 2)) return 1;
#endif
      return slex_consume_single_char(ctx, SLEX_TOK_period);
    case '&':
      if (slex_try_match(ctx, SLEX_TOK_and, "&&", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_bitwise_and_eq, "&=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_bitwise_and);
    case '*':
      if (slex_try_match(ctx, SLEX_TOK_mul_eq, "*=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_mul);
    case '+':
      if (slex_try_match(ctx, SLEX_TOK_inc, "++", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_plus_eq, "+=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_plus);
    case '-':
      if (slex_try_match(ctx, SLEX_TOK_arrow, "->", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_dec, "--", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_minus_eq, "-=", 2)) return 1;
#if SLEX_ADD_CXX_SUPPORT
      if (slex_try_match(ctx, SLEX_TOK_deref_access, "->*", 3)) return 1;
#endif
      return slex_consume_single_char(ctx, SLEX_TOK_minus);
    case '~':
      return slex_consume_single_char(ctx, SLEX_TOK_bitwise_not);
    case '!':
      if (slex_try_match(ctx, SLEX_TOK_not_eq, "!=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_not);
    case '/':
      if (slex_try_match(ctx, SLEX_TOK_div_eq, "/=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_div);
    case '%':
      if (slex_try_match(ctx, SLEX_TOK_mod_eq, "%=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_mod);
    case '<':
      if (slex_try_match(ctx, SLEX_TOK_spaceship, "<=>", 3)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_shl_eq, "<<=", 3)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_less_or_eq, "<=", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_shl, "<<", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_less);
    case '>':
      if (slex_try_match(ctx, SLEX_TOK_shr_eq, ">>=", 3)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_greater_or_eq, ">=", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_shr, ">>", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_greater);
    case '^':
      if (slex_try_match(ctx, SLEX_TOK_xor_eq, "^=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_xor);
    case '|':
      if (slex_try_match(ctx, SLEX_TOK_or, "||", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_bitwise_or_eq, "|=", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_bitwise_or);
    case '?':
      return slex_consume_single_char(ctx, SLEX_TOK_questionmark);
    case ':':
#if SLEX_ADD_CXX_SUPPORT
      if (slex_try_match(ctx, SLEX_TOK_scope_resolution, "::", 2)) return 1;
#endif
      return slex_consume_single_char(ctx, SLEX_TOK_colon);
    case ';':
      return slex_consume_single_char(ctx, SLEX_TOK_semi);
    case '=':
      if (slex_try_match(ctx, SLEX_TOK_equality, "==", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_assign);
    case ',':
      return slex_consume_single_char(ctx, SLEX_TOK_comma);
    case '#':
      if (slex_try_match(ctx, SLEX_TOK_concat, "##", 2)) return 1;
      if (slex_try_match(ctx, SLEX_TOK_preprocessor_at, "#@", 2)) return 1;
      return slex_consume_single_char(ctx, SLEX_TOK_preprocessor);
    default:
      return 0; // Not a punctuator
  }
}

static int slex_skip(SlexContext *ctx) {
  while(ctx->parse_point < ctx->stream_end) {
    // preprocessor
#if SLEX_SKIP_PREPROCESSOR
    if(*ctx->parse_point == '#') {
      while(ctx->parse_point < ctx->stream_end) {
        if(*ctx->parse_point == '\n') break;
        ctx->parse_point++;
      }
      ctx->parse_point++;
      continue;
    }
#endif
    // whitespace
    if(slex_is_whitespace(*ctx->parse_point)) {
      while(ctx->parse_point < ctx->stream_end) {
        if(!slex_is_whitespace(*ctx->parse_point)) break;
        ctx->parse_point++;
      }
      continue;
    }

    // is last char
    if(ctx->parse_point >= ctx->stream_end - 1) {
#if SLEX_SKIP_PREPROCESSOR
      if(*ctx->parse_point == '#') ctx->parse_point++;
#endif
      break;
    }

    // comments
    if(*ctx->parse_point == '/' && ctx->parse_point[1] == '/') {
      ctx->parse_point += 2; // skip //
      while(ctx->parse_point < ctx->stream_end) {
        if(*ctx->parse_point == '\n') break;
        ctx->parse_point++;
      }
      ctx->parse_point++;
      continue;
    }
    else if(*ctx->parse_point == '/' && ctx->parse_point[1] == '*') {
      ctx->parse_point += 2;
      int terminated = 0;
      while(ctx->parse_point < ctx->stream_end) {
        if(*ctx->parse_point == '*' 
            && ctx->parse_point <= ctx->stream_end - 2
            && ctx->parse_point[1] == '/') {
          ctx->parse_point += 2;
          terminated = 1;
          break;
        }
        ctx->parse_point++;
      }
      if(!terminated)
        return slex_return_err(SLEX_ERR_parse, ctx);
      continue;
    }
    else break; // Neither whitespace nor comments nor preprocessor
  }
  return 1;
}

static int slex_parse_ident(SlexContext *ctx) {
  ctx->tok_ty = SLEX_TOK_identifier;
  ctx->first_tok_char = ctx->parse_point;

  while(ctx->parse_point < ctx->stream_end) {
    if(!slex_is_ident(*ctx->parse_point)) break;
    ctx->parse_point++;
  }

  ctx->last_tok_char = ctx->parse_point - 1;
  return 1;
}

static int slex_parse_num(SlexContext *ctx) {
  char *end = ctx->parse_point;
  unsigned long long fact = 0;
  while(end < ctx->stream_end) {
    if(!slex_is_numeric(*end)) break;

    if(slex_mul_overflows_u64(fact, 10)) {
      ctx->parse_point = end;
      return slex_return_err(SLEX_ERR_storage, ctx);
    }
    end++;
    if(fact != 0) fact *= 10;
    else fact = 1;
  }

  unsigned long long num = 0;

  for(char *it = ctx->parse_point; it < end; it++) {
    unsigned long long n = (*it - '0') * fact;

    if(slex_add_overflows_u64(n, num)) {
      ctx->parse_point = it;
      return slex_return_err(SLEX_ERR_storage, ctx);
    }
    num += n;
    fact /= 10;
  }
  ctx->parsed_int_lit = num;
  ctx->parse_point = end;
  ctx->last_tok_char = end - 1;

#if SLEX_PARSE_INT_SUFFIXES
  slex_parse_int_suffix(ctx);
#endif
  return 1;
}

static int slex_parse_int_lit(SlexContext *ctx) {
  ctx->first_tok_char = ctx->parse_point;
  ctx->tok_ty = SLEX_TOK_int_lit;

  if(*ctx->parse_point != '0') 
    return slex_parse_num(ctx);

  // the current char is the last one
  if(ctx->parse_point >= ctx->stream_end - 1) goto zero;

  // hexadecimals
  if(ctx->parse_point[1] == 'x' || ctx->parse_point[1] == 'X') {
    ctx->parse_point += 2; // consume 0x

    if(ctx->parse_point >= ctx->stream_end) 
      return slex_return_err(SLEX_ERR_parse, ctx);

    char *end = ctx->parse_point;
    unsigned long long fact = 0;

    while(end < ctx->stream_end) {
      if(!slex_is_hex(*end)) break;

      if(slex_mul_overflows_u64(fact, 16)) {
        ctx->parse_point = end;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      end++;
      if(fact != 0) fact *= 16;
      else fact = 1;
    }

    if(end - ctx->parse_point == 0) 
      return slex_return_err(SLEX_ERR_parse, ctx);

    unsigned long long hex = 0;

    for(char *it = ctx->parse_point; it < end; it++) {
      unsigned long long h = slex_hex_to_int(*it) * fact;
      if(slex_add_overflows_u64(h, hex)) {
        ctx->parse_point = it;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      hex += h;
      fact /= 16;
    }

    ctx->parsed_int_lit = hex;
    ctx->parse_point = end;
    ctx->last_tok_char = end - 1;
#if SLEX_PARSE_INT_SUFFIXES
    slex_parse_int_suffix(ctx);
#endif
    return 1;
  }

  // octals
  if(slex_is_numeric(ctx->parse_point[1])) {
    char *end = ctx->parse_point;
    unsigned long long fact = 0;

    while(end < ctx->stream_end) {
      if(!slex_is_oct(*end)) break;

      if(slex_mul_overflows_u64(fact, 8)) {
        ctx->parse_point = end;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      end++;
      if(fact != 0) fact *= 8;
      else fact = 1;
    }

    unsigned long long oct = 0;

    for(char *it = ctx->parse_point; it < end; it++) {
      unsigned long long o = (*it - '0') * fact;

      if(slex_add_overflows_u64(o, oct)) {
        ctx->parse_point = it;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      oct += o;
      fact /= 8;
    }

    ctx->parsed_int_lit = oct;
    ctx->parse_point = end;
    ctx->last_tok_char = end - 1;
#if SLEX_PARSE_INT_SUFFIXES
    slex_parse_int_suffix(ctx);
#endif
    return 1;
  }

  if(ctx->parse_point[1] == 'b') {
    ctx->parse_point += 2; // consume 0b

    if(ctx->parse_point >= ctx->stream_end) 
      return slex_return_err(SLEX_ERR_parse, ctx);

    char *end = ctx->parse_point;
    unsigned long long fact = 0;

    while(end < ctx->stream_end) {
      if(*end != '0' && *end != '1') break;

      if(slex_mul_overflows_u64(fact, 2)) {
        ctx->parse_point = end;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      end++;
      if(fact != 0) fact *= 2;
      else fact = 1;
    }

    if(end - ctx->parse_point == 0) 
      return slex_return_err(SLEX_ERR_parse, ctx);

    unsigned long long bin = 0;

    for(char *it = ctx->parse_point; it < end; it++) {
      unsigned long long b = (*it - '0') * fact;
      if(slex_add_overflows_u64(b, bin)) {
        ctx->parse_point = it;
        return slex_return_err(SLEX_ERR_storage, ctx);
      }
      bin += b;
      fact /= 2;
    }

    ctx->parsed_int_lit = bin;
    ctx->parse_point = end;
    ctx->last_tok_char = end - 1;
#if SLEX_PARSE_INT_SUFFIXES
    slex_parse_int_suffix(ctx);
#endif
    return 1;
  }

zero:
  ctx->parsed_int_lit = 0;
  ctx->last_tok_char = ctx->first_tok_char;
  ctx->parse_point++;
#if SLEX_PARSE_INT_SUFFIXES
  slex_parse_int_suffix(ctx);
#endif
  return 1;
}

static long long slex_parse_esc_seq(SlexContext *ctx) {
  ctx->parse_point++; // consume \

  if(ctx->parse_point >= ctx->stream_end) 
    return slex_return_err(SLEX_ERR_parse, ctx) -1;

  if(slex_is_oct(*ctx->parse_point)) {
    char *end = ctx->parse_point;
    int fact = 0;
    int i = 0;

    while(end < ctx->stream_end) {
      if(!slex_is_oct(*end) || i == 3) break;
      end++; i++;
      if(fact != 0) fact *= 8;
      else fact = 1;
    }

    int oct = 0;

    for(char *it = ctx->parse_point; it < end; it++) {
      int o = (*it - '0') * fact;
      oct += o;
      fact /= 8;
    }

    ctx->parse_point = end;
    return oct;
  }

  if(*ctx->parse_point == 'x') {
    if(ctx->parse_point > ctx->stream_end - 3 || !slex_is_hex(ctx->parse_point[1])) {
      ctx->parse_point++;
      return slex_return_err(SLEX_ERR_parse, ctx) -1;
    }
    if(!slex_is_hex(ctx->parse_point[2])) {
      ctx->parse_point += 2;
      return slex_return_err(SLEX_ERR_parse, ctx) -1;
    }
    int hex = slex_hex_to_int(ctx->parse_point[1]) * 16 + slex_hex_to_int(ctx->parse_point[2]); 
    ctx->parse_point += 3;
    return hex;
  }

  if(*ctx->parse_point == 'u' || *ctx->parse_point == 'U') {
    int len = *ctx->parse_point == 'u' ? 4 : 8;
    ctx->parse_point++; // consume u or U

    if(ctx->parse_point >= ctx->stream_end) 
      return slex_return_err(SLEX_ERR_parse, ctx);

    char *end = ctx->parse_point;
    long long fact = 0;
    int i = 0;

    while(end < ctx->stream_end && i < len) {
      if(!slex_is_hex(*end)) break;
      end++; i++;
      if(fact != 0) fact *= 16;
      else fact = 1;
    }

    if(i != len) {
      ctx->parse_point = end;
      return slex_return_err(SLEX_ERR_parse, ctx) -1;
    }

    long long codepoint = 0;

    for(char *it = ctx->parse_point; it < end; it++) {
      codepoint += slex_hex_to_int(*it) * fact;
      fact /= 16;
    }

    ctx->parse_point = end;
    return codepoint;
  }

other_sequence:
  ctx->parse_point++;
  switch(ctx->parse_point[-1]) {
    default: ctx->parse_point--; return slex_return_err(SLEX_ERR_parse, ctx) -1;
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    case '\\': return '\\';
    case '\'': return '\'';
    case '"': return '"';
    case '?': return '\?';
  }
}

static int slex_parse_char_or_str_lit(SlexContext *ctx) {
  int curr_str_idx = 0;

  char delim = *ctx->parse_point;
  ctx->tok_ty = delim == '"' ? SLEX_TOK_str_lit : SLEX_TOK_char_lit;
  ctx->first_tok_char = ctx->parse_point;

  ctx->parse_point++; // consume " or '

  while(ctx->parse_point < ctx->stream_end) {
    if(*ctx->parse_point == delim) {
      ctx->last_tok_char = ctx->parse_point;
      ctx->str_len = curr_str_idx;
      ctx->parse_point++;
      return 1;
    }

    if(curr_str_idx >= ctx->string_store_len) 
      return slex_return_err(SLEX_ERR_storage, ctx);

    if(*ctx->parse_point == '\\') {
      long long c = slex_parse_esc_seq(ctx);
      if(c == -1) return 0;

      int len = slex_utf8_encode_esc_seq(ctx, c, ctx->string_store + curr_str_idx);
      if(len == -1)
        return slex_return_err(SLEX_ERR_storage, ctx);

      curr_str_idx += len;
    }
    else {
      ctx->string_store[curr_str_idx] = *ctx->parse_point;
      ctx->parse_point++;
      curr_str_idx++;
    }
  }

  return slex_return_err(SLEX_ERR_parse, ctx);
}

void slex_init_context(SlexContext *ctx, char *stream_start,
    char *stream_end, char* string_store, int string_store_len) {
  ctx->parse_point = stream_start;
  ctx->stream_end = stream_end;
  ctx->string_store = string_store;
  ctx->string_store_len = string_store_len;
}

int slex_get_next_token(SlexContext *ctx) {
  if(ctx->parse_point >= ctx->stream_end) 
    return slex_return_eof(ctx);

  if(!slex_skip(ctx))
    return 0;

  if(ctx->parse_point >= ctx->stream_end) 
    return slex_return_eof(ctx);

  // numbers
  if(slex_is_numeric(*ctx->parse_point))
    return slex_parse_int_lit(ctx);

  // string literals
  if(*ctx->parse_point == '"' || *ctx->parse_point == '\'')
    return slex_parse_char_or_str_lit(ctx);

  // identifier
  if(slex_is_ident(*ctx->parse_point)) 
    return slex_parse_ident(ctx);

  // punctuators
  if(slex_parse_punctuator(ctx))
    return 1;


  return slex_return_err(SLEX_ERR_unknown_tok, ctx);
}

void slex_get_token_location(SlexContext *ctx, char *stream_begin, int *line_num, int *col_num) {
  int ln = 1;
  int col = 1;

  for(char *it = stream_begin; it < ctx->first_tok_char; it++) {
    if (*it == '\n') {
      ln++;
      col = 1;
    } else {
      col++;
    }
  }
  *line_num = ln;
  *col_num = col;
}

void slex_get_parse_ptr_location(SlexContext *ctx, char *stream_begin, int *line_num, int *col_num) {
  int ln = 1;
  int col = 1;

  for(char *it = stream_begin; it < ctx->parse_point; it++) {
    if (*it == '\n') {
      ln++;
      col = 1;
    } else {
      col++;
    }
  }
  *line_num = ln;
  *col_num = col;
}
#endif // SLEX_IMPLEMENTATION
#endif // SLEX_H
