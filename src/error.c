#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include "jcc.h"

int enable_warnings = 1;

void error(const char *filename, int line_num, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: %d: %s", filename, line_num,
            isatty(fileno(stderr)) ? "\x1b[1;31merror: \x1b[0m" : "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

void warning(const char *filename, int line_num, char *fmt, ...)
{
    if (!enable_warnings) return;
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: %d: %s ", filename, line_num,
            isatty(fileno(stderr)) ? "\x1b[1;33mwarning: \x1b[0m" : "warning: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
