#ifndef ERROR_H
#define ERROR_H

void error(const char *filename, int line_num, char *fmt, ...);
void warning(const char *filename, int line_num, char *fmt, ...);

#endif
