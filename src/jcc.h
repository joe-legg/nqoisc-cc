#ifndef JCC_H
#define JCC_H

#include <stdio.h>

// error.c
void error(const char *filename, int line_num, char *fmt, ...);
void warning(const char *filename, int line_num, char *fmt, ...);

// Parser
void parse(char *filename);

#endif
