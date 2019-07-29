#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdarg.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
void sprintf(char *str, char *fmt, ...);
int vsprintf(char *str, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
