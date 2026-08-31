#ifndef SIBC_UI_H
#define SIBC_UI_H

#include <stdbool.h>
#include <stdio.h>

// ANSI Colors & Styles
#define UI_RESET     "\033[0m"
#define UI_BOLD      "\033[1m"
#define UI_DIM       "\033[2m"
#define UI_ITALIC    "\033[3m"
#define UI_UNDERLINE "\033[4m"

#define UI_RED     "\033[38;5;196m"
#define UI_GREEN   "\033[38;5;46m"
#define UI_YELLOW  "\033[38;5;220m"
#define UI_BLUE    "\033[38;5;39m"
#define UI_MAGENTA "\033[38;5;201m"
#define UI_CYAN    "\033[38;5;51m"
#define UI_GRAY    "\033[38;5;242m"
#define UI_WHITE   "\033[38;5;255m"

// Status Badges
#define UI_TAG_INFO    UI_BOLD UI_BLUE "  INFO   " UI_RESET " "
#define UI_TAG_SUCCESS UI_BOLD UI_GREEN "    OK   " UI_RESET " "
#define UI_TAG_WARN    UI_BOLD UI_YELLOW "  WARN   " UI_RESET " "
#define UI_TAG_ERROR   UI_BOLD UI_RED " ERROR   " UI_RESET " "
#define UI_TAG_STAGE   UI_BOLD UI_MAGENTA " STAGE   " UI_RESET " "

void ui_banner(void);
void ui_version(const char *ver);
void ui_help(const char *prog);
void ui_stage(const char *name, const char *detail);
void ui_success(const char *msg);
void ui_error(const char *msg);

#endif // SIBC_UI_H
