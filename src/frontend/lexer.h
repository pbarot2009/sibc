#ifndef SIBC_LEXER_H
#define SIBC_LEXER_H

#include "token.h"

typedef struct Lexer {
  const char *source;
  size_t cursor;
  uint32_t line;
  uint32_t col;
  const char *filename;
} Lexer;

// Scans entire source into an arr_push stretchy buffer of Tokens
Token *lexer_tokenize(const char *source, const char *filename);

// Helper to get printable string for any token kind (for debugging/errors)
const char *token_kind_name(TokenKind kind);

#endif // SIBC_LEXER_H
