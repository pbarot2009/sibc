#include "ui.h"

#include <stdio.h>

void ui_banner(void) {
  printf("\n" UI_BOLD UI_CYAN "sibc" UI_RESET " " UI_DIM "::" UI_RESET " " UI_WHITE
         "Something is Better C" UI_RESET "\n");
  printf(UI_DIM "------------------------------------------" UI_RESET "\n");
}

void ui_version(const char *ver) {
  ui_banner();
  printf("  " UI_DIM "Version :" UI_RESET "  " UI_BOLD UI_GREEN "%s" UI_RESET "\n", ver);
  printf("  " UI_DIM "Target  :" UI_RESET "  " UI_WHITE "C17 (Verified Safe Output)" UI_RESET "\n");
  printf("  " UI_DIM "License :" UI_RESET "  " UI_YELLOW "Apache-2.0" UI_RESET "\n");
  printf(UI_DIM "------------------------------------------" UI_RESET "\n\n");
}

void ui_help(const char *prog) {
  ui_banner();
  printf("  " UI_BOLD "USAGE:" UI_RESET "\n");
  printf("    " UI_CYAN "%s" UI_RESET " " UI_YELLOW "<command>" UI_RESET " [flags] " UI_WHITE
         "<input.c>" UI_RESET "\n\n",
         prog);

  printf("  " UI_BOLD "COMMANDS:" UI_RESET "\n");
  printf("    " UI_GREEN "build" UI_RESET
         "        Compile, verify safety, and emit C17 / binary\n");
  printf("    " UI_GREEN "run" UI_RESET "          Verify, compile, and execute program\n");
  printf("    " UI_GREEN "check" UI_RESET "        Run safety analysis without code emission\n");
  printf("    " UI_GREEN "version" UI_RESET "      Show compiler version and info\n");
  printf("    " UI_GREEN "help" UI_RESET "         Show this help screen\n\n");

  printf("  " UI_BOLD "FLAGS:" UI_RESET "\n");
  printf("    " UI_CYAN "-o" UI_RESET " " UI_DIM "<path>" UI_RESET
         "     Output path or directory\n");
  printf("    " UI_CYAN "--release" UI_RESET "     Enable compiler release optimizations\n");
  printf("    " UI_CYAN "--tokenize" UI_RESET "    Display token stream and exit\n");
  printf("    " UI_CYAN "--ast" UI_RESET "         Dump parsed syntax tree\n");
  printf("    " UI_CYAN "--time" UI_RESET "        Measure compiler pass timings\n");
  printf("    " UI_CYAN "-v, --version" UI_RESET " Show version\n");
  printf("    " UI_CYAN "-h, --help" UI_RESET "    Show help\n");
  printf(UI_DIM "------------------------------------------" UI_RESET "\n\n");
}

void ui_stage(const char *name, const char *detail) {
  printf(UI_TAG_STAGE UI_BOLD "%-10s" UI_RESET " " UI_DIM "->" UI_RESET " %s\n", name, detail);
}

void ui_success(const char *msg) {
  printf(UI_TAG_OK UI_GREEN "%s" UI_RESET "\n", msg);
}

void ui_error(const char *msg) {
  fprintf(stderr, UI_TAG_ERR UI_RED "%s" UI_RESET "\n", msg);
}
