#include "kernel.h"

size_t
strlen(const char *s)
{
	size_t n;

	for (n = 0; s[n] != '\0'; n++);

	return n;
}

void *
memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *p1, *p2, *end;

	p1 = (unsigned char *)dest;
	p2 = (unsigned char *)src;
	end = p1 + n;

	while (p1 != end)
		*p1++ = *p2++;

	return dest;
}

void *
memset(void *s, int c, size_t n)
{
	unsigned char *p, *end;

	p = s;
	end = p + n;

	while (p != end)
		*p++ = c;

	return s;
}

