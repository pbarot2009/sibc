#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC "clang"
#define TARGET "bin/sibc"
#define SRC_MAIN "src/main.c"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

static void run_cmd(const char *cmd) {
  printf("%s[CMD]%s %s\n", COLOR_BLUE, COLOR_RESET, cmd);
  int code = system(cmd);
  if (code != 0) {
    fprintf(stderr, "%s[ERROR]%s Command failed with code %d\n", COLOR_RED,
            COLOR_RESET, code);
    exit(EXIT_FAILURE);
  }
}

static void ensure_dir(const char *dir) {
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "mkdir -p %s", dir);
  system(cmd);
}

int main(int argc, char **argv) {
  const char *mode = (argc > 1) ? argv[1] : "debug";
  ensure_dir("bin");

  if (strcmp(mode, "clean") == 0) {
    run_cmd("rm -rf bin build");
    printf("%s[CLEAN]%s Output directories wiped.\n", COLOR_GREEN, COLOR_RESET);
    return 0;
  }

  char cmd[512];

  if (strcmp(mode, "release") == 0) {
    // Optimized release binary
    snprintf(cmd, sizeof(cmd),
             "%s -std=c11 -O3 -flto -DNDEBUG -Wall -Wextra -Isrc %s -o %s", CC,
             SRC_MAIN, TARGET);
  } else {
    // Fast debug build with debug symbols and runtime safety
    snprintf(
        cmd, sizeof(cmd),
        "%s -std=c11 -O0 -g -Wall -Wextra -Wno-unused-parameter -Isrc %s -o %s",
        CC, SRC_MAIN, TARGET);
  }

  run_cmd(cmd);
  printf("%s[SUCCESS]%s Built target '%s' (%s mode)\n", COLOR_GREEN,
         COLOR_RESET, TARGET, mode);

  // If 'run' argument passed: ./build.sh run tests/example.c
  if (strcmp(mode, "run") == 0) {
    char run_cmd_buf[512];
    const char *pass_arg = (argc > 2) ? argv[2] : "";
    snprintf(run_cmd_buf, sizeof(run_cmd_buf), "./%s %s", TARGET, pass_arg);
    run_cmd(run_cmd_buf);
  }

  return 0;
}
