#include "core/arena.c"
#include "core/arena.h"
#include "core/array.h"
#include "frontend/lexer.c"
#include "frontend/lexer.h"
#include "frontend/token.h"

#include <stdio.h>

int main(void) {
  const char *source_code = "// sibc lexer verification\n"
                            "int main(void) {\n"
                            "    int * _Nonnull p = 0x2A;\n"
                            "    defer free(p);\n"
                            "    return 100 + 20;\n"
                            "}";

  printf("Tokenizing source code...\n\n");
  Token *tokens = lexer_tokenize(source_code, "test.c");

  for (size_t i = 0; i < arr_len(tokens); i++) {
    Token t = tokens[i];
    printf("[%u:%u] %-15s '%.*s'\n", t.loc.line, t.loc.col, token_kind_name(t.kind), (int) t.length,
           t.start);
  }

  arr_free(tokens);
  return 0;
}
