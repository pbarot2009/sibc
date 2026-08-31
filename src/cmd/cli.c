#include "cli.h"
#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli_print_version(void) {
  ui_version(SIBC_VERSION);
}

void cli_print_help(const char *prog_name) {
  ui_help(prog_name);
}

CliOptions cli_parse(int argc, char **argv) {
  CliOptions opt = {0};
  opt.output_file = "out";

  if (argc < 2) {
    cli_print_help(argv[0]);
    opt.exit_code = 1;
    return opt;
  }

  const char *cmd_str = argv[1];

  if (strcmp(cmd_str, "version") == 0 || strcmp(cmd_str, "-v") == 0 ||
      strcmp(cmd_str, "--version") == 0) {
    opt.cmd = CMD_VERSION;
    return opt;
  }

  if (strcmp(cmd_str, "help") == 0 || strcmp(cmd_str, "-h") == 0 ||
      strcmp(cmd_str, "--help") == 0) {
    opt.cmd = CMD_HELP;
    return opt;
  }

  if (strcmp(cmd_str, "build") == 0)
    opt.cmd = CMD_BUILD;
  else if (strcmp(cmd_str, "run") == 0)
    opt.cmd = CMD_RUN;
  else if (strcmp(cmd_str, "check") == 0)
    opt.cmd = CMD_CHECK;
  else {
    char err_msg[128];
    snprintf(err_msg, sizeof(err_msg), "Unknown command '%s'", cmd_str);
    ui_error(err_msg);
    cli_print_help(argv[0]);
    opt.exit_code = 1;
    return opt;
  }

  for (int i = 2; i < argc; i++) {
    const char *arg = argv[i];

    if (strcmp(arg, "--tokenize") == 0) {
      opt.flag_tokenize = true;
    } else if (strcmp(arg, "--ast") == 0) {
      opt.flag_ast = true;
    } else if (strcmp(arg, "--time") == 0) {
      opt.flag_time = true;
    } else if (strcmp(arg, "--release") == 0) {
      opt.flag_release = true;
    } else if (strcmp(arg, "-o") == 0) {
      if (i + 1 >= argc) {
        ui_error("Missing parameter for flag '-o'");
        opt.exit_code = 1;
        return opt;
      }
      opt.output_file = argv[++i];
    } else if (arg[0] == '-') {
      char err_msg[128];
      snprintf(err_msg, sizeof(err_msg), "Unrecognized flag '%s'", arg);
      ui_error(err_msg);
      opt.exit_code = 1;
      return opt;
    } else {
      if (opt.input_file != NULL) {
        ui_error("Multiple input files are not supported in single build");
        opt.exit_code = 1;
        return opt;
      }
      opt.input_file = arg;
    }
  }

  if (!opt.input_file && opt.cmd != CMD_HELP && opt.cmd != CMD_VERSION) {
    ui_error("No input source file provided");
    opt.exit_code = 1;
  }

  return opt;
}
