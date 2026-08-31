#ifndef SIBC_UI_H
#define SIBC_UI_H

#include <stdbool.h>
#include <stdio.h>

// ANSI Colors & Text Attributes
#define UI_RESET "\033[0m"
#define UI_BOLD  "\033[1m"
#define UI_DIM   "\033[2m"

#define UI_RED    "\033[38;5;196m"
#define UI_GREEN  "\033[38;5;46m"
#define UI_YELLOW "\033[38;5;220m"
#define UI_BLUE   "\033[38;5;39m"
#define UI_CYAN   "\033[38;5;51m"
#define UI_WHITE  "\033[38;5;255m"

// Status Badges
#define UI_TAG_STAGE UI_BOLD UI_BLUE "[STAGE]" UI_RESET " "
#define UI_TAG_OK    UI_BOLD UI_GREEN "[  OK  ]" UI_RESET " "
#define UI_TAG_WARN  UI_BOLD UI_YELLOW "[ WARN ]" UI_RESET " "
#define UI_TAG_ERR   UI_BOLD UI_RED "[ FAIL ]" UI_RESET " "

void ui_banner(void);
void ui_version(const char *ver);
void ui_help(const char *prog);
void ui_stage(const char *name, const char *detail);
void ui_success(const char *msg);
void ui_error(const char *msg);

#endif // SIBC_UI_H
