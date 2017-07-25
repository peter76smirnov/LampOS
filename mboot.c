#include "kernel.h"
#include "stdio.h"

#define MB_MASK0	(1<<0)
#define MB_MASK1	(1<<1)
#define MB_MASK2	(1<<2)

void
mb_parseinfo(void *mbinfo)
{
	uint32_t flags;
	struct {
		uint32_t lower;
		uint32_t upper;
	} *memory;
	struct {
		uint8_t drive;
		uint8_t part1;
		uint8_t part2;
		uint8_t part3;
	} *boot_device;
	char **cmdline;

	flags = *(uint32_t *)mbinfo;

	kprintf("parsing multiboot mbinfo...\n");
	if (flags & MB_MASK0) {
		memory = mbinfo + 4;

		kprintf("mem_lower %X\n", memory->lower);
		kprintf("mem_upper %X\n", memory->upper);
	}
	if (flags & MB_MASK1) {
		boot_device = mbinfo + 12;

		kprintf("drive %X\n", boot_device->drive);
		kprintf("part1 %X\n", boot_device->part1);
		kprintf("part2 %X\n", boot_device->part2);
		kprintf("part3 %X\n", boot_device->part3);
	}
	if (flags & MB_MASK2) {
		cmdline = mbinfo + 16;

		kprintf("cmdline %s\n", *cmdline);
	}

	kputc('\n');
}

void
mb_normalizemmap(void *mmap)
{
	;
}

