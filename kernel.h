#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdint.h>
#include <stddef.h>

#include "defs.h"

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


#define NELEMS(p) (sizeof(p) / sizeof(p[0]))


size_t strlen(const char *s);

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

#endif
