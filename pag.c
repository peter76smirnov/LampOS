#include <stdint.h>

#include "kernel.h"
#include "stdio.h"

#define PG_PRESENT	(1 << 0)
#define PG_RW		(1 << 1)
#define PG_US		(1 << 2)
#define PG_ACCESS	(1 << 5)
#define PG_DIRTY	(1 << 6)

uint32_t page_directory[1024]	__attribute__ ((aligned(0x1000)));
uint32_t page_table[1024]	__attribute__ ((aligned(0x1000)));

extern void load_pgdir(void *);

void
pag_init()
{
	int i;

	/* Identity paging first 4M */
	page_directory[0] = (uint32_t)page_table | 0x3;
	for (i = 0; i < 1024; i++) {
		page_table[i] = (i * 0x1000) | 0x3;
	}

	/* Self-referencing dir entry */
	page_directory[1023] = (uint32_t)page_directory | 0x3;

	kprintf("pgdir address\t%X\n\n", page_directory);
	kprintf("enabling paging...\n");

	load_pgdir(&page_directory);

	kprintf("Hello, paging world!\n\n");
}

