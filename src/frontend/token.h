#ifndef SIBC_TOKEN_H
#define SIBC_TOKEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum TokenKind {
  TOK_EOF = 0,
  TOK_INVALID,

  // Literals & Identifiers
  TOK_IDENT,
  TOK_INT_LIT,
  TOK_FLOAT_LIT,
  TOK_STRING_LIT,
  TOK_CHAR_LIT,

  // Single-character Symbols
  TOK_LPAREN,    // (
  TOK_RPAREN,    // )
  TOK_LBRACKET,  // [
  TOK_RBRACKET,  // ]
  TOK_LBRACE,    // {
  TOK_RBRACE,    // }
  TOK_SEMICOLON, // ;
  TOK_COMMA,     // ,
  TOK_DOT,       // .
  TOK_QUESTION,  // ?
  TOK_COLON,     // :
  TOK_TILDE,     // ~
  TOK_HASH,      // #

  // Operators & Multi-char Symbols
  TOK_ASSIGN,  // =
  TOK_PLUS,    // +
  TOK_MINUS,   // -
  TOK_STAR,    // *
  TOK_SLASH,   // /
  TOK_PERCENT, // %
  TOK_AMP,     // &
  TOK_PIPE,    // |
  TOK_CARET,   // ^
  TOK_BANG,    // !
  TOK_LT,      // <
  TOK_GT,      // >

  TOK_EQ,       // ==
  TOK_NE,       // !=
  TOK_LE,       // <=
  TOK_GE,       // >=
  TOK_AND,      // &&
  TOK_OR,       // ||
  TOK_INC,      // ++
  TOK_DEC,      // --
  TOK_ARROW,    // ->
  TOK_SHL,      // <<
  TOK_SHR,      // >>
  TOK_ELLIPSIS, // ...

  // Compound Assignment
  TOK_ADD_ASSIGN, // +=
  TOK_SUB_ASSIGN, // -=
  TOK_MUL_ASSIGN, // *=
  TOK_DIV_ASSIGN, // /=
  TOK_MOD_ASSIGN, // %=
  TOK_SHL_ASSIGN, // <<=
  TOK_SHR_ASSIGN, // >>=
  TOK_AND_ASSIGN, // &=
  TOK_XOR_ASSIGN, // ^=
  TOK_OR_ASSIGN,  // |=

  // Standard Keywords
  TOK_KW_AUTO,
  TOK_KW_BREAK,
  TOK_KW_CASE,
  TOK_KW_CHAR,
  TOK_KW_CONST,
  TOK_KW_CONTINUE,
  TOK_KW_DEFAULT,
  TOK_KW_DO,
  TOK_KW_DOUBLE,
  TOK_KW_ELSE,
  TOK_KW_ENUM,
  TOK_KW_EXTERN,
  TOK_KW_FLOAT,
  TOK_KW_FOR,
  TOK_KW_GOTO,
  TOK_KW_IF,
  TOK_KW_INLINE,
  TOK_KW_INT,
  TOK_KW_LONG,
  TOK_KW_REGISTER,
  TOK_KW_RESTRICT,
  TOK_KW_RETURN,
  TOK_KW_SHORT,
  TOK_KW_SIGNED,
  TOK_KW_SIZEOF,
  TOK_KW_STATIC,
  TOK_KW_STRUCT,
  TOK_KW_SWITCH,
  TOK_KW_TYPEDEF,
  TOK_KW_UNION,
  TOK_KW_UNSIGNED,
  TOK_KW_VOID,
  TOK_KW_VOLATILE,
  TOK_KW_WHILE,
  TOK_KW_BOOL,

  // sibc Safety Extensions
  TOK_KW_DEFER,
  TOK_KW_NONNULL,
  TOK_KW_NULLABLE,
} TokenKind;

typedef struct SourceLoc {
  uint32_t line;
  uint32_t col;
  const char *filename;
} SourceLoc;

typedef struct Token {
  TokenKind kind;
  SourceLoc loc;
  const char *start; // Points directly into the source file buffer
  size_t length;
  union {
    int64_t int_val;
    double float_val;
    char char_val;
  };
} Token;

#endif // SIBC_TOKEN_H
