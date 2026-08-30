#include "cmd/cli.c"
#include "cmd/cli.h"
#include "core/arena.c"
// #include "core/arena.h"
#include "core/array.h"
#include "frontend/lexer.c"
#include "frontend/lexer.h"
#include "frontend/token.h"

#include <stdio.h>
#include <stdlib.h>

static char *read_file_to_string(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    return NULL;

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char *buf = (char *) malloc(size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  fread(buf, 1, size, f);
  buf[size] = '\0';
  fclose(f);
  return buf;
}

int main(int argc, char **argv) {
  CliOptions opt = cli_parse(argc, argv);
  if (opt.exit_code != 0)
    return opt.exit_code;

  if (opt.cmd == CMD_VERSION) {
    cli_print_version();
    return 0;
  }

  if (opt.cmd == CMD_HELP) {
    cli_print_help(argv[0]);
    return 0;
  }

  char *source = read_file_to_string(opt.input_file);
  if (!source) {
    fprintf(stderr, "error: failed to read file '%s'\n", opt.input_file);
    return 1;
  }

  // 1. Lexical Analysis
  Token *tokens = lexer_tokenize(source, opt.input_file);

  if (opt.flag_tokenize) {
    for (size_t i = 0; i < arr_len(tokens); i++) {
      Token t = tokens[i];
      printf("[%u:%u] %-15s '%.*s'\n", t.loc.line, t.loc.col, token_kind_name(t.kind),
             (int) t.length, t.start);
    }
    arr_free(tokens);
    free(source);
    return 0;
  }

  // Pipeline dispatch
  if (opt.cmd == CMD_CHECK) {
    printf("Checking %s...\n", opt.input_file);
  } else if (opt.cmd == CMD_BUILD) {
    printf("Building %s -> %s\n", opt.input_file, opt.output_file);
  } else if (opt.cmd == CMD_RUN) {
    printf("Building and running %s...\n", opt.input_file);
  }

  arr_free(tokens);
  free(source);
  return 0;
}
