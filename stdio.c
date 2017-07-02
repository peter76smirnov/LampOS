#include <stdint.h>

#include "stdio.h"
#include "kernel.h"
#include "vga.h"

char buffer[4096];
int current;

int
kputc(int c)
{
	vga_putc((char)c);

	return c;
}

int
kputs(const char *s)
{
	kprintf("%s\n", s);

	return 0;
}

size_t
_itoa(const char *alph, char *buf, unsigned int val, int base)
{
	size_t len;
	int i;
	int j;
	char tmp;

	if (val == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		return 1;
	}

	for (i = 0; val > 0; i++) {
		buf[i] = alph[val % base];
		val /= base;
	}
	len = i;
	buf[len] = '\0';

	for (j = 0, i--; j < i; j++, i--) {
		tmp = buf[i];
		buf[i] = buf[j];
		buf[j] = tmp;
	}

	return len;
}

int
_cp2buffer(char *src, size_t n)
{
	if (current + n >= 4096) {
		/* set errno to EOVERFLOW */
		return 1;
	}

	memcpy(buffer+current, src, n);
	current += n;

	return 0;
}

int
kprintf(const char *fmt, ...)
{
	va_list args;

	memset(buffer, 0, sizeof buffer);
	current = 0;
	va_start(args, fmt);

	while (*fmt != '\0') {
		char buf[13];
		size_t len;
		char c;
		char *s;
		int inum;
		unsigned int num;
		char *alph = "0123456789abcdef";
		int sign = 0;

		if (*fmt != '%')
			goto print;

		fmt++;
		switch (*fmt) {
		case 'c':
			c = va_arg(args, char);

			if (_cp2buffer(&c, 1)) {
				return -1;
			}

			break;
		case 's':
			s = va_arg(args, char *);

			if (_cp2buffer(s, strlen(s))) {
				return -1;
			}

			break;
		case 'd':
		case 'i':
			inum = va_arg(args, int);
			if (inum < 0) {
				inum *= -1;
				sign = 1;
			}

			len = _itoa(alph, buf, (unsigned int)inum, 10);
			if (sign) {
				c = '-';
				_cp2buffer(&c, 1);
			}
			if (_cp2buffer(buf, len)) {
				return -1;
			}

			break;
		case 'u':
			num = va_arg(args, unsigned int);

			len = _itoa(alph, buf, num, 10);
			if (_cp2buffer(buf, len)) {
				return -1;
			}

			break;
		case 'X':
			alph = "0123456789ABCDEF";
		case 'x':
			num = va_arg(args, unsigned int);

			len = _itoa(alph, buf, num, 16);
			if (_cp2buffer(buf, len)) {
				return -1;
			}

			break;
		case 'o':
			num = va_arg(args, unsigned int);

			len = _itoa(alph, buf, num, 8);
			if (_cp2buffer(buf, len)) {
				return -1;
			}

			break;
		case '%':
		default:
			goto print;
		}

		fmt++;
		continue;

print:
		if (_cp2buffer(fmt, 1))
			return -1;
		fmt++;
	}

		vga_puts(buffer);

		return current;
}

