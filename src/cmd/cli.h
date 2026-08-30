#ifndef SIBC_CLI_H
#define SIBC_CLI_H

#include <stdbool.h>
#include <stddef.h>

#define SIBC_VERSION "0.0.1-dev"

typedef enum CmdKind {
  CMD_NONE = 0,
  CMD_HELP,
  CMD_VERSION,
  CMD_BUILD,
  CMD_RUN,
  CMD_CHECK,
} CmdKind;

typedef struct CliOptions {
  CmdKind cmd;
  const char *input_file;
  const char *output_file;
  bool flag_tokenize;
  bool flag_ast;
  bool flag_time;
  bool flag_release;
  int exit_code;
} CliOptions;

CliOptions cli_parse(int argc, char **argv);
void cli_print_help(const char *prog_name);
void cli_print_version(void);

#endif // SIBC_CLI_H
