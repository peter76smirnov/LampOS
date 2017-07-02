#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

int kputc(int c);

int kputs(const char *s);

int kprintf(const char *fmt, ...);

#endif
