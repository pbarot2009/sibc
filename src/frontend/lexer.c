#include "../core/array.h"
#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_ident_start(char c) {
  return isalpha((unsigned char) c) || c == '_';
}

static bool is_ident_body(char c) {
  return isalnum((unsigned char) c) || c == '_';
}

static char lexer_peek(Lexer *l) {
  return l->source[l->cursor];
}

static char lexer_peek_next(Lexer *l) {
  if (l->source[l->cursor] == '\0')
    return '\0';
  return l->source[l->cursor + 1];
}

static char lexer_advance(Lexer *l) {
  char c = l->source[l->cursor++];
  if (c == '\n') {
    l->line++;
    l->col = 1;
  } else {
    l->col++;
  }
  return c;
}

static bool lexer_match(Lexer *l, char expected) {
  if (lexer_peek(l) == expected) {
    lexer_advance(l);
    return true;
  }
  return false;
}

static void skip_whitespace_and_comments(Lexer *l) {
  while (true) {
    char c = lexer_peek(l);
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
      lexer_advance(l);
    } else if (c == '/' && lexer_peek_next(l) == '/') {
      // Line comment
      while (lexer_peek(l) != '\0' && lexer_peek(l) != '\n') {
        lexer_advance(l);
      }
    } else if (c == '/' && lexer_peek_next(l) == '*') {
      // Block comment
      lexer_advance(l); // /
      lexer_advance(l); // *
      while (lexer_peek(l) != '\0') {
        if (lexer_peek(l) == '*' && lexer_peek_next(l) == '/') {
          lexer_advance(l);
          lexer_advance(l);
          break;
        }
        lexer_advance(l);
      }
    } else {
      break;
    }
  }
}

static TokenKind check_keyword(const char *str, size_t len) {
#define MATCH_KW(name, tok)                                                                        \
  if (len == sizeof(name) - 1 && memcmp(str, name, len) == 0)                                      \
  return tok

  switch (str[0]) {
  case '_':
    MATCH_KW("_Bool", TOK_KW_BOOL);
    MATCH_KW("_Nonnull", TOK_KW_NONNULL);
    MATCH_KW("_Nullable", TOK_KW_NULLABLE);
    break;
  case 'a':
    MATCH_KW("auto", TOK_KW_AUTO);
    break;
  case 'b':
    MATCH_KW("break", TOK_KW_BREAK);
    break;
  case 'c':
    MATCH_KW("case", TOK_KW_CASE);
    MATCH_KW("char", TOK_KW_CHAR);
    MATCH_KW("const", TOK_KW_CONST);
    MATCH_KW("continue", TOK_KW_CONTINUE);
    break;
  case 'd':
    MATCH_KW("default", TOK_KW_DEFAULT);
    MATCH_KW("defer", TOK_KW_DEFER);
    MATCH_KW("do", TOK_KW_DO);
    MATCH_KW("double", TOK_KW_DOUBLE);
    break;
  case 'e':
    MATCH_KW("else", TOK_KW_ELSE);
    MATCH_KW("enum", TOK_KW_ENUM);
    MATCH_KW("extern", TOK_KW_EXTERN);
    break;
  case 'f':
    MATCH_KW("float", TOK_KW_FLOAT);
    MATCH_KW("for", TOK_KW_FOR);
    break;
  case 'g':
    MATCH_KW("goto", TOK_KW_GOTO);
    break;
  case 'i':
    MATCH_KW("if", TOK_KW_IF);
    MATCH_KW("inline", TOK_KW_INLINE);
    MATCH_KW("int", TOK_KW_INT);
    break;
  case 'l':
    MATCH_KW("long", TOK_KW_LONG);
    break;
  case 'r':
    MATCH_KW("register", TOK_KW_REGISTER);
    MATCH_KW("restrict", TOK_KW_RESTRICT);
    MATCH_KW("return", TOK_KW_RETURN);
    break;
  case 's':
    MATCH_KW("short", TOK_KW_SHORT);
    MATCH_KW("signed", TOK_KW_SIGNED);
    MATCH_KW("sizeof", TOK_KW_SIZEOF);
    MATCH_KW("static", TOK_KW_STATIC);
    MATCH_KW("struct", TOK_KW_STRUCT);
    MATCH_KW("switch", TOK_KW_SWITCH);
    break;
  case 't':
    MATCH_KW("typedef", TOK_KW_TYPEDEF);
    break;
  case 'u':
    MATCH_KW("union", TOK_KW_UNION);
    MATCH_KW("unsigned", TOK_KW_UNSIGNED);
    break;
  case 'v':
    MATCH_KW("void", TOK_KW_VOID);
    MATCH_KW("volatile", TOK_KW_VOLATILE);
    break;
  case 'w':
    MATCH_KW("while", TOK_KW_WHILE);
    break;
  }
#undef MATCH_KW
  return TOK_IDENT;
}

static Token scan_identifier_or_keyword(Lexer *l) {
  SourceLoc loc = {.line = l->line, .col = l->col, .filename = l->filename};
  const char *start = &l->source[l->cursor];

  while (is_ident_body(lexer_peek(l))) {
    lexer_advance(l);
  }

  size_t length = (size_t) (&l->source[l->cursor] - start);
  TokenKind kind = check_keyword(start, length);

  return (Token) {
      .kind = kind,
      .loc = loc,
      .start = start,
      .length = length,
  };
}

static Token scan_number(Lexer *l) {
  SourceLoc loc = {.line = l->line, .col = l->col, .filename = l->filename};
  const char *start = &l->source[l->cursor];
  bool is_float = false;

  // Hex, Binary, or Octal/Decimal
  if (lexer_peek(l) == '0') {
    lexer_advance(l);
    if (lexer_peek(l) == 'x' || lexer_peek(l) == 'X') {
      lexer_advance(l);
      while (isxdigit((unsigned char) lexer_peek(l)))
        lexer_advance(l);
      goto finalize_int;
    } else if (lexer_peek(l) == 'b' || lexer_peek(l) == 'B') {
      lexer_advance(l);
      while (lexer_peek(l) == '0' || lexer_peek(l) == '1')
        lexer_advance(l);
      goto finalize_int;
    }
  }

  while (isdigit((unsigned char) lexer_peek(l))) {
    lexer_advance(l);
  }

  if (lexer_peek(l) == '.' && isdigit((unsigned char) lexer_peek_next(l))) {
    is_float = true;
    lexer_advance(l); // .
    while (isdigit((unsigned char) lexer_peek(l))) {
      lexer_advance(l);
    }
  }

  if (lexer_peek(l) == 'e' || lexer_peek(l) == 'E') {
    is_float = true;
    lexer_advance(l);
    if (lexer_peek(l) == '+' || lexer_peek(l) == '-')
      lexer_advance(l);
    while (isdigit((unsigned char) lexer_peek(l)))
      lexer_advance(l);
  }

finalize_int:
  // Consume integer/float suffixes (e.g., U, L, LL, UL, f, F)
  while (isalnum((unsigned char) lexer_peek(l))) {
    lexer_advance(l);
  }

  size_t length = (size_t) (&l->source[l->cursor] - start);
  Token token = {
      .loc = loc,
      .start = start,
      .length = length,
  };

  if (is_float) {
    token.kind = TOK_FLOAT_LIT;
    token.float_val = strtod(start, NULL);
  } else {
    token.kind = TOK_INT_LIT;
    token.int_val = (int64_t) strtoll(start, NULL, 0);
  }

  return token;
}

static Token scan_string(Lexer *l) {
  SourceLoc loc = {.line = l->line, .col = l->col, .filename = l->filename};
  lexer_advance(l); // Opening quote "
  const char *start = &l->source[l->cursor];

  while (lexer_peek(l) != '"' && lexer_peek(l) != '\0') {
    if (lexer_peek(l) == '\\') {
      lexer_advance(l); // Skip escape char
    }
    lexer_advance(l);
  }

  size_t length = (size_t) (&l->source[l->cursor] - start);
  if (lexer_peek(l) == '"') {
    lexer_advance(l); // Closing quote "
  }

  return (Token) {
      .kind = TOK_STRING_LIT,
      .loc = loc,
      .start = start,
      .length = length,
  };
}

static Token scan_char(Lexer *l) {
  SourceLoc loc = {.line = l->line, .col = l->col, .filename = l->filename};
  lexer_advance(l); // Opening quote '
  char val = 0;

  if (lexer_peek(l) == '\\') {
    lexer_advance(l);
    char esc = lexer_advance(l);
    switch (esc) {
    case 'n':
      val = '\n';
      break;
    case 't':
      val = '\t';
      break;
    case 'r':
      val = '\r';
      break;
    case '0':
      val = '\0';
      break;
    case '\\':
      val = '\\';
      break;
    case '\'':
      val = '\'';
      break;
    default:
      val = esc;
      break;
    }
  } else {
    val = lexer_advance(l);
  }

  if (lexer_peek(l) == '\'') {
    lexer_advance(l); // Closing quote '
  }

  return (Token) {
      .kind = TOK_CHAR_LIT,
      .loc = loc,
      .char_val = val,
      .length = 1,
  };
}

Token *lexer_tokenize(const char *source, const char *filename) {
  Lexer l = {
      .source = source,
      .cursor = 0,
      .line = 1,
      .col = 1,
      .filename = filename,
  };

  Token *tokens = NULL;

  while (true) {
    skip_whitespace_and_comments(&l);

    SourceLoc loc = {.line = l.line, .col = l.col, .filename = l.filename};
    const char *start = &l.source[l.cursor];
    char c = lexer_peek(&l);

    if (c == '\0') {
      Token tok = {.kind = TOK_EOF, .loc = loc, .start = start, .length = 0};
      arr_push(tokens, tok);
      break;
    }

    if (is_ident_start(c)) {
      arr_push(tokens, scan_identifier_or_keyword(&l));
      continue;
    }

    if (isdigit((unsigned char) c)) {
      arr_push(tokens, scan_number(&l));
      continue;
    }

    if (c == '"') {
      arr_push(tokens, scan_string(&l));
      continue;
    }

    if (c == '\'') {
      arr_push(tokens, scan_char(&l));
      continue;
    }

    lexer_advance(&l);
    TokenKind kind = TOK_INVALID;

    switch (c) {
    case '(':
      kind = TOK_LPAREN;
      break;
    case ')':
      kind = TOK_RPAREN;
      break;
    case '[':
      kind = TOK_LBRACKET;
      break;
    case ']':
      kind = TOK_RBRACKET;
      break;
    case '{':
      kind = TOK_LBRACE;
      break;
    case '}':
      kind = TOK_RBRACE;
      break;
    case ';':
      kind = TOK_SEMICOLON;
      break;
    case ',':
      kind = TOK_COMMA;
      break;
    case '~':
      kind = TOK_TILDE;
      break;
    case '?':
      kind = TOK_QUESTION;
      break;
    case ':':
      kind = TOK_COLON;
      break;
    case '#':
      kind = TOK_HASH;
      break;

    case '.':
      if (lexer_peek(&l) == '.' && lexer_peek_next(&l) == '.') {
        lexer_advance(&l);
        lexer_advance(&l);
        kind = TOK_ELLIPSIS;
      } else {
        kind = TOK_DOT;
      }
      break;

    case '+':
      if (lexer_match(&l, '+'))
        kind = TOK_INC;
      else if (lexer_match(&l, '='))
        kind = TOK_ADD_ASSIGN;
      else
        kind = TOK_PLUS;
      break;

    case '-':
      if (lexer_match(&l, '-'))
        kind = TOK_DEC;
      else if (lexer_match(&l, '>'))
        kind = TOK_ARROW;
      else if (lexer_match(&l, '='))
        kind = TOK_SUB_ASSIGN;
      else
        kind = TOK_MINUS;
      break;

    case '*':
      kind = lexer_match(&l, '=') ? TOK_MUL_ASSIGN : TOK_STAR;
      break;
    case '/':
      kind = lexer_match(&l, '=') ? TOK_DIV_ASSIGN : TOK_SLASH;
      break;
    case '%':
      kind = lexer_match(&l, '=') ? TOK_MOD_ASSIGN : TOK_PERCENT;
      break;
    case '^':
      kind = lexer_match(&l, '=') ? TOK_XOR_ASSIGN : TOK_CARET;
      break;
    case '=':
      kind = lexer_match(&l, '=') ? TOK_EQ : TOK_ASSIGN;
      break;
    case '!':
      kind = lexer_match(&l, '=') ? TOK_NE : TOK_BANG;
      break;

    case '<':
      if (lexer_match(&l, '<')) {
        kind = lexer_match(&l, '=') ? TOK_SHL_ASSIGN : TOK_SHL;
      } else if (lexer_match(&l, '=')) {
        kind = TOK_LE;
      } else {
        kind = TOK_LT;
      }
      break;

    case '>':
      if (lexer_match(&l, '>')) {
        kind = lexer_match(&l, '=') ? TOK_SHR_ASSIGN : TOK_SHR;
      } else if (lexer_match(&l, '=')) {
        kind = TOK_GE;
      } else {
        kind = TOK_GT;
      }
      break;

    case '&':
      if (lexer_match(&l, '&'))
        kind = TOK_AND;
      else if (lexer_match(&l, '='))
        kind = TOK_AND_ASSIGN;
      else
        kind = TOK_AMP;
      break;

    case '|':
      if (lexer_match(&l, '|'))
        kind = TOK_OR;
      else if (lexer_match(&l, '='))
        kind = TOK_OR_ASSIGN;
      else
        kind = TOK_PIPE;
      break;

    default:
      kind = TOK_INVALID;
      break;
    }

    Token tok = {
        .kind = kind,
        .loc = loc,
        .start = start,
        .length = (size_t) (&l.source[l.cursor] - start),
    };
    arr_push(tokens, tok);
  }

  return tokens;
}

const char *token_kind_name(TokenKind kind) {
  switch (kind) {
  case TOK_EOF:
    return "EOF";
  case TOK_INVALID:
    return "INVALID";
  case TOK_IDENT:
    return "IDENTIFIER";
  case TOK_INT_LIT:
    return "INT_LIT";
  case TOK_FLOAT_LIT:
    return "FLOAT_LIT";
  case TOK_STRING_LIT:
    return "STRING_LIT";
  case TOK_CHAR_LIT:
    return "CHAR_LIT";
  case TOK_LPAREN:
    return "(";
  case TOK_RPAREN:
    return ")";
  case TOK_LBRACKET:
    return "[";
  case TOK_RBRACKET:
    return "]";
  case TOK_LBRACE:
    return "{";
  case TOK_RBRACE:
    return "}";
  case TOK_SEMICOLON:
    return ";";
  case TOK_COMMA:
    return ",";
  case TOK_DOT:
    return ".";
  case TOK_ASSIGN:
    return "=";
  case TOK_PLUS:
    return "+";
  case TOK_MINUS:
    return "-";
  case TOK_STAR:
    return "*";
  case TOK_SLASH:
    return "/";
  case TOK_PERCENT:
    return "%";
  case TOK_EQ:
    return "==";
  case TOK_NE:
    return "!=";
  case TOK_ARROW:
    return "->";
  case TOK_KW_INT:
    return "kw_int";
  case TOK_KW_RETURN:
    return "kw_return";
  case TOK_KW_IF:
    return "kw_if";
  case TOK_KW_DEFER:
    return "kw_defer";
  case TOK_KW_NONNULL:
    return "kw__Nonnull";
  case TOK_KW_NULLABLE:
    return "kw__Nullable";
  default:
    return "TOKEN";
  }
}
