#ifndef _KERN_ARCH_I386_
#define _KERN_ARCH_I386_

#define VGA_MEMORY	0xB8000
#define VGA_HEIGHT	25
#define VGA_WIDTH	80

#define VGA_COLOR_BLACK		0x0
#define VGA_COLOR_BLUE		0x1
#define VGA_COLOR_GREEN		0x2
#define VGA_COLOR_CYAN		0x3
#define VGA_COLOR_RED		0x4
#define VGA_COLOR_MAGENTA	0x5
#define VGA_COLOR_BROWN		0x6
#define VGA_COLOR_LIGHT_GREY	0x7
#define VGA_COLOR_DARK_GREY	0x8
#define VGA_COLOR_LIGHT_BLUE	0x9
#define VGA_COLOR_LIGHT_GREEN	0xa
#define VGA_COLOR_LIGHT_CYAN	0xb
#define VGA_COLOR_LIGHT_RED	0xc
#define VGA_COLOR_LIGHT_MAGENTA	0xd
#define VGA_COLOR_LIGHT_BROWN	0xe
#define VGA_COLOR_WHITE		0xf

static uint16_t
vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static uint8_t
vga_entrycolor(uint8_t fg, uint8_t bg)
{
	return fg | bg << 4;
}

#endif

