#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli_print_version(void) {
  printf("sibc version %s\n", SIBC_VERSION);
}

void cli_print_help(const char *prog_name) {
  printf("Usage: %s <command> [options] <input_file.c>\n\n", prog_name);
  printf("Commands:\n");
  printf("  build    Compile source and emit verified C17 / binary\n");
  printf("  run      Compile and immediately execute the output\n");
  printf("  check    Verify memory safety and syntax without emitting code\n");
  printf("  version  Print version information\n");
  printf("  help     Print this help menu\n\n");
  printf("Flags:\n");
  printf("  --tokenize       Run lexer only and print token stream\n");
  printf("  --ast            Run parser only and dump the AST\n");
  printf("  --time           Measure and print compilation pass timings\n");
  printf("  --release        Enable release optimizations\n");
  printf("  -o <file>        Specify output path (default: out/)\n");
  printf("  -h, --help       Print help\n");
  printf("  -v, --version    Print version\n");
}

CliOptions cli_parse(int argc, char **argv) {
  CliOptions opt = {0};
  opt.output_file = "out";

  if (argc < 2) {
    cli_print_help(argv[0]);
    opt.exit_code = 1;
    return opt;
  }

  const char *first_arg = argv[1];

  if (strcmp(first_arg, "version") == 0 || strcmp(first_arg, "-v") == 0 ||
      strcmp(first_arg, "--version") == 0) {
    opt.cmd = CMD_VERSION;
    return opt;
  }

  if (strcmp(first_arg, "help") == 0 || strcmp(first_arg, "-h") == 0 ||
      strcmp(first_arg, "--help") == 0) {
    opt.cmd = CMD_HELP;
    return opt;
  }

  if (strcmp(first_arg, "build") == 0) {
    opt.cmd = CMD_BUILD;
  } else if (strcmp(first_arg, "run") == 0) {
    opt.cmd = CMD_RUN;
  } else if (strcmp(first_arg, "check") == 0) {
    opt.cmd = CMD_CHECK;
  } else {
    fprintf(stderr, "error: unknown command '%s'\n\n", first_arg);
    cli_print_help(argv[0]);
    opt.exit_code = 1;
    return opt;
  }

  // Parse flags and input file
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
        fprintf(stderr, "error: missing argument after '-o'\n");
        opt.exit_code = 1;
        return opt;
      }
      opt.output_file = argv[++i];
    } else if (arg[0] == '-') {
      fprintf(stderr, "error: unknown flag '%s'\n", arg);
      opt.exit_code = 1;
      return opt;
    } else {
      if (opt.input_file != NULL) {
        fprintf(stderr, "error: multiple input files provided ('%s' and '%s')\n", opt.input_file,
                arg);
        opt.exit_code = 1;
        return opt;
      }
      opt.input_file = arg;
    }
  }

  if (!opt.input_file && opt.cmd != CMD_HELP && opt.cmd != CMD_VERSION) {
    fprintf(stderr, "error: no input file provided\n");
    opt.exit_code = 1;
  }

  return opt;
}
