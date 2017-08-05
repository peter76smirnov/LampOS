#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef	__linux__
#error	"you are not using cross-compiler"
#endif

#ifndef	__i386__
#error	"it needs to be compiled with ix86 compiler"
#endif


/* VGA text buffer memory mapped address */
#define VGA_BUFFER_ADDR		0xB8000

/*  */
#define VGA_WIDTH		80
#define VGA_HEIGHT		25

/* VGA palette colors */
#define VGA_COLOR_BRIGHT 	0x8

#define VGA_COLOR_BLACK		0x0
#define VGA_COLOR_BLUE		0x1
#define VGA_COLOR_GREEN		0x2
#define VGA_COLOR_CYAN		0x3
#define VGA_COLOR_RED		0x4
#define VGA_COLOR_MAGENTA	0x5
#define VGA_COLOR_BROWN		0x6
#define VGA_COLOR_LIGHT_GREY	0x7
#define VGA_COLOR_DARK_GREY	(VGA_COLOR_BRIGHT | VGA_COLOR_BLACK)
#define VGA_COLOR_LIGHT_BLUE	(VGA_COLOR_BRIGHT | VGA_COLOR_BLUE)
#define VGA_COLOR_LIGHT_GREEN	(VGA_COLOR_BRIGHT | VGA_COLOR_GREEN)
#define VGA_COLOR_LIGHT_CYAN	(VGA_COLOR_BRIGHT | VGA_COLOR_CYAN)
#define VGA_COLOR_LIGHT_RED	(VGA_COLOR_BRIGHT | VGA_COLOR_RED)
#define VGA_COLOR_LIGHT_MAGENTA	(VGA_COLOR_BRIGHT | VGA_COLOR_MAGENTA)
#define VGA_COLOR_LIGHT_BROWN	(VGA_COLOR_BRIGHT | VGA_COLOR_BROWN)
#define VGA_COLOR_WHITE		(VGA_COLOR_BRIGHT | VGA_COLOR_LIGHT_GREY)

static inline uint8_t
vga_entry_color(uint8_t fg, uint8_t bg)
{
	return fg | bg<<4;
}

static inline uint16_t
vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color<<8;
}

size_t
strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

size_t term_row;
size_t term_column;
uint8_t term_color;
uint16_t *term_buffer;

void
term_scroll_up();

void
term_init()
{
	term_row = 0;
	term_column = 0;
	term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term_buffer = (uint16_t *) VGA_BUFFER_ADDR;

	size_t x, y;
	for (y = 0; y < VGA_HEIGHT; y++)
		for (x = 0; x < VGA_WIDTH; x++)
			term_buffer[y*VGA_WIDTH + x] = vga_entry(' ', term_color);
}

void
term_set_color(uint8_t color)
{
	term_color = color;
}

void
term_put_entry_at(char c, uint8_t color, size_t y, size_t x)
{
	term_buffer[y*VGA_WIDTH + x] = vga_entry(c, color);
}

void
term_put_char(char c)
{
	if (c == '\n') {
		term_column = 0;
		if (++term_row == VGA_HEIGHT) {
			term_row--;
			term_scroll_up();
		}
		return;
	}
	term_put_entry_at(c, term_color, term_row, term_column);
	if (++term_column == VGA_WIDTH) {
		term_column = 0;
		if (++term_row == VGA_HEIGHT) {
			term_row--;
			term_scroll_up();
		}
	}
}

void
term_write(const char *data, size_t siz)
{
	size_t i = 0;
	for (i = 0; i < siz; i++)
		term_put_char(data[i]);
}

void
term_write_string(const char *data)
{
	term_write(data, strlen(data));
}

void
term_scroll_up()
{
	size_t i;

	for (i = 0; i < VGA_HEIGHT-1; i++) {
		size_t j;
		for (j = 0; j < VGA_WIDTH; j++)
			term_buffer[i*VGA_WIDTH + j] = term_buffer[(i+1)*VGA_WIDTH + j];
	}
	for (i = 0; i < VGA_WIDTH; i++)
		term_put_entry_at(' ', term_color, VGA_HEIGHT-1, i);
}

void
kernel_main()
{
	term_init();

	term_write_string("Hello, world!\n");
	term_write_string("THIS IS LAMPO$\n");

	term_write_string("\tHEY\t");

/*
	term_set_color(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));

	term_put_entry_at('*', term_color, 10, 20);
	term_put_entry_at('*', term_color, 10, 24);
	term_put_entry_at('*', term_color, 11, 20);
	term_put_entry_at('*', term_color, 11, 24);
	term_put_entry_at('*', term_color, 12, 20);
	term_put_entry_at('*', term_color, 12, 21);
	term_put_entry_at('*', term_color, 12, 22);
	term_put_entry_at('*', term_color, 12, 23);
	term_put_entry_at('*', term_color, 12, 24);
	term_put_entry_at('*', term_color, 13, 20);
	term_put_entry_at('*', term_color, 13, 24);
	term_put_entry_at('*', term_color, 14, 20);
	term_put_entry_at('*', term_color, 14, 24);

	term_put_entry_at('*', term_color, 10, 26);
	term_put_entry_at('*', term_color, 10, 27);
	term_put_entry_at('*', term_color, 10, 28);
	term_put_entry_at('*', term_color, 10, 29);
	term_put_entry_at('*', term_color, 10, 30);
	term_put_entry_at('*', term_color, 11, 26);
	term_put_entry_at('*', term_color, 12, 26);
	term_put_entry_at('*', term_color, 12, 27);
	term_put_entry_at('*', term_color, 12, 28);
	term_put_entry_at('*', term_color, 13, 26);
	term_put_entry_at('*', term_color, 14, 26);
	term_put_entry_at('*', term_color, 14, 27);
	term_put_entry_at('*', term_color, 14, 28);
	term_put_entry_at('*', term_color, 14, 29);
	term_put_entry_at('*', term_color, 14, 30);

	term_put_entry_at('*', term_color, 10, 32);
	term_put_entry_at('*', term_color, 11, 32);
	term_put_entry_at('*', term_color, 12, 32);
	term_put_entry_at('*', term_color, 13, 32);
	term_put_entry_at('*', term_color, 14, 32);
	term_put_entry_at('*', term_color, 14, 33);
	term_put_entry_at('*', term_color, 14, 34);
	term_put_entry_at('*', term_color, 14, 35);
	term_put_entry_at('*', term_color, 14, 36);

	term_put_entry_at('*', term_color, 10, 38);
	term_put_entry_at('*', term_color, 11, 38);
	term_put_entry_at('*', term_color, 12, 38);
	term_put_entry_at('*', term_color, 13, 38);
	term_put_entry_at('*', term_color, 14, 38);
	term_put_entry_at('*', term_color, 14, 39);
	term_put_entry_at('*', term_color, 14, 40);
	term_put_entry_at('*', term_color, 14, 41);
	term_put_entry_at('*', term_color, 14, 42);

	term_put_entry_at('*', term_color, 10, 44);
	term_put_entry_at('*', term_color, 10, 45);
	term_put_entry_at('*', term_color, 10, 46);
	term_put_entry_at('*', term_color, 10, 47);
	term_put_entry_at('*', term_color, 10, 48);
	term_put_entry_at('*', term_color, 11, 44);
	term_put_entry_at('*', term_color, 11, 48);
	term_put_entry_at('*', term_color, 12, 44);
	term_put_entry_at('*', term_color, 12, 48);
	term_put_entry_at('*', term_color, 13, 44);
	term_put_entry_at('*', term_color, 13, 48);
	term_put_entry_at('*', term_color, 14, 44);
	term_put_entry_at('*', term_color, 14, 45);
	term_put_entry_at('*', term_color, 14, 46);
	term_put_entry_at('*', term_color, 14, 47);
	term_put_entry_at('*', term_color, 14, 48);
*/
/*
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_RED);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BROWN);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BROWN);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLUE);
	term_scroll_up();
	term_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_MAGENTA);
	term_scroll_up();
*/
}

