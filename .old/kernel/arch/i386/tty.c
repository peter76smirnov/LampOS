#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static size_t	 term_x;
static size_t	 term_y;
static uint8_t	 term_color;
static uint16_t *term_buffer;

void
term_setcolor(uint8_t color)
{
	term_color = color;
}

void
vga_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	term_buffer[y*VGA_WIDTH + x] = vga_getentry(c, color);
}

void
term_init()
{
	term_x = 0;
	term_y = 0;
	term_color = vga_getcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term_buffer = (void *) VGA_MEMORY;

	vga_clearscreen();
}

void
term_putc(char c)
{
	uint16_t entry = vga_entry(c, term_color);

	term_buffer[y*VGA_WIDTH + x] = entry;
	if (++term_x == VGA_WIDTH) {
		term_x = 0;
		if (++term_y == VGA_HEIGHT) {
			term_y = 0;
			term_scroll();
		}
	}
}

void
term_write(const char *data, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; i++)
		term_putc(data[i]);
}

