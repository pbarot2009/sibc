#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC       "clang"
#define TARGET   "bin/sibc"
#define SRC_MAIN "src/main.c"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE  "\033[1;34m"
#define COLOR_RED   "\033[1;31m"
#define COLOR_RESET "\033[0m"

static void run_cmd(const char *cmd) {
  printf("%s[CMD]%s %s\n", COLOR_BLUE, COLOR_RESET, cmd);
  int code = system(cmd);
  if (code != 0) {
    fprintf(stderr, "%s[ERROR]%s Command failed with code %d\n", COLOR_RED, COLOR_RESET, code);
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

  // Clean output directory
  if (strcmp(mode, "clean") == 0) {
    run_cmd("rm -rf bin");
    printf("%s[CLEAN]%s Output directory wiped.\n", COLOR_GREEN, COLOR_RESET);
    return 0;
  }

  // Format all C files in src/ and root
  if (strcmp(mode, "fmt") == 0) {
    run_cmd(
        "clang-format -i src/*.c src/*.h build.c 2>/dev/null || clang-format -i src/*.c build.c");
    printf("%s[FMT]%s Formatted codebase.\n", COLOR_GREEN, COLOR_RESET);
    return 0;
  }

  char cmd[512];

  if (strcmp(mode, "release") == 0) {
    // Fast, lightweight binary with no sanitizer bloat
    snprintf(cmd, sizeof(cmd),
             "%s -std=c17 -O3 -flto -DNDEBUG -Wall -Wextra -Wpedantic -Isrc %s -o %s", CC, SRC_MAIN,
             TARGET);
  } else {
    // Debug mode: ASan + UBSan for compiler safety
    snprintf(cmd, sizeof(cmd),
             "%s -std=c17 -O0 -g3 -Wall -Wextra -Wpedantic "
             "-fsanitize=address,undefined -fno-omit-frame-pointer "
             "-fno-sanitize-recover=all -Isrc %s -o %s",
             CC, SRC_MAIN, TARGET);
  }

  run_cmd(cmd);
  printf("%s[SUCCESS]%s Built target '%s' (%s mode)\n", COLOR_GREEN, COLOR_RESET, TARGET, mode);

  // Execute directly with ./build.sh run <args>
  if (strcmp(mode, "run") == 0) {
    char run_cmd_buf[512];
    const char *pass_arg = (argc > 2) ? argv[2] : "";
    snprintf(run_cmd_buf, sizeof(run_cmd_buf), "./%s %s", TARGET, pass_arg);
    run_cmd(run_cmd_buf);
  }

  return 0;
}
