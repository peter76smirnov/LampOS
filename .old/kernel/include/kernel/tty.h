#ifndef _KERNEL_TTY_H_
#define _KERNEL_TTY_H_

#include <stddef.h>

void
term_init(void);

void
term_putc(char);

void
term_write(const char *, size_t);

#endif
