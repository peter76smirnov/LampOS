#include <stdint.h>

#include "kernel.h"
#include "stdio.h"

#define PG_PRESENT	(1 << 0)
#define PG_RW		(1 << 1)
#define PG_US		(1 << 2)
#define PG_ACCESS	(1 << 5)
#define PG_DIRTY	(1 << 6)

static uint32_t page_directory[1024];

extern void load_page_directory(void *page_directory);

void
paging_init()
{
	uint32_t *pte;
	uint32_t address;
	int i;

	/* Identity paging first 4M */
	address = 0x100000;
	pte = &page_directory[0];
	for (i = 0; i < 1024; i++, address += 0x1000, pte++) {
		*pte = address | 0x3;

		kprintf("pt[%d] %X\n", i, *pte);
	}

/*
	load_page_directory(&page_directory);
*/
}

