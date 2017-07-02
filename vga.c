#include <stdint.h>

#include "kernel.h"
#include "vga.h"

static unsigned int width = VGA_WIDTH, height = VGA_HEIGHT;
static int x_coord, y_coord;
static int fg_color, bg_color;
static uint16_t *buffer = NULL;
static uint32_t tabstp[8];

void
vga_init()
{
	int i;

	x_coord = y_coord = 0;
	bg_color = VGA_COLOR_BLACK;
	fg_color = VGA_COLOR_LIGHT_GREY;
	buffer = (void *)VGA_MEMORY;

	for (i = 0; i < NELEMS(tabstp); i++)
		tabstp[i] = 0x01010101;

	vga_clear_screen();
}

uint16_t
vga_make_char(unsigned char c, uint8_t fg, uint8_t bg)
{
	uint16_t res;

	res = (fg | bg<<4) << 8;
	res |= c;

	return res;
}

void
vga_clear_screen()
{
	int x, y;

	if (buffer == NULL)
		return;

	x_coord = 0;
	y_coord = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			buffer[y*width + x] = 
			    vga_make_char(' ', fg_color, bg_color);
		}
	}
}

void
vga_scroll_up()
{
	int x, y;

	if (buffer == NULL)
		return;

	for (y = 0; y < height-1; y++) {
		for (x = 0; x < width; x++) {
			buffer[y*width + x] = buffer[(y+1)*width + x];
		}
	}
	for (x = 0; x < width; x++)
		buffer[y*width + x] = vga_make_char(' ', fg_color, bg_color);
}

void
vga_putc(char c)
{
	int x;

	if (buffer == NULL)
		return;

	switch (c) {
	case '\n':
		x_coord = 0;
		y_coord++;
		break;
	case '\b':
		if (x_coord > 0) {
			x_coord--;
			buffer[y_coord*width + x_coord] =
			    vga_make_char(' ', fg_color, bg_color);
		}
		break;
	case '\r':
		x_coord = 0;
		break;
	case '\t':
		for (x = x_coord+1; x < width; x++) {
			if (tabstp[x/32] & (1 << (x%32))) {
				x_coord = x;
				break;
			}
		}
		break;
	default:
		buffer[y_coord*width + x_coord] = 
		    vga_make_char(c, fg_color, bg_color);
		x_coord++;
	}

	if (x_coord >= width ) {
		x_coord = 0;
		y_coord++;
	}
	if (y_coord >= height) {
		vga_scroll_up();
		y_coord--;
	}
}

void
vga_puts(char *s)
{
	while (*s != '\0')
		vga_putc(*s++);
}

