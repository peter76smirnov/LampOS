#include <stdint.h>

#include "kernel.h"

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
	uint32_t *first_pte;
	uint32_t address;
	int i;

	first_pte = &page_directory[0];
	address = 0x0;
	for (i = 0; i < 1024; i++, address += 0x1000, first_pte++) {
		*first_pte = address | 0x7;	/* PRESENT READ-WRITE ALL */
//		kprintf("pt[%d] %X\n", i, *first_pte);
	}

//	load_page_directory(&page_directory);
}
