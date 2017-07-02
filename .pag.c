#include <stdint.h>

#include "stdio.h"
#include "kernel.h"

#define PG_PRESENT	(1 << 0)
#define PG_RW		(1 << 1)
#define PG_US		(1 << 2)
#define PG_ACCESS	(1 << 5)
#define PG_DIRTY	(1 << 6)

struct page_entry {
	uint16_t flags : 12;
	uint32_t addr : 20;
} __attribute__ ((packed));

static struct page_entry page_directory[1024];

extern void load_page_directory(struct page_entry *page_directory);

void
page_table_set(unsigned index, uint32_t addr, uint8_t flags)
{
	struct page_entry *page_table;

	page_table = &page_directory[index];

	page_table->addr = addr;

	/* 00000000UR1 */
	page_table->flags = PG_PRESENT;
	page_table->flags |= flags & PG_RW;
	page_table->flags |= flags & PG_US;

	kprintf("index: %u addr: %x flags: %x\n",
	    index, page_table->addr, page_table->flags);
}

void
paging_init()
{
	struct page_entry *pgtable;
	int i;
	uint32_t address;

/*
	page_table_set(0, 0x100000, PG_RW|PG_US);
*/

	address = 0x100000;		/* 1M */

	page_directory[0].addr = address;
	page_directory[0].flags = PG_PRESENT | PG_RW | PG_US;

	kprintf("pd[0]\t\taddr %X\t\t flags %X\n",
	    page_directory[0].addr, page_directory[0].flags);

	for (i = 0; i < 257; i++, address += 0x1000) {
		kprintf("\taddress %X\n", address);
		pgtable = (struct page_entry *)(page_directory[0].addr + i*4);
		pgtable->addr = address;
		pgtable->flags = PG_PRESENT | PG_RW | PG_US;

			kprintf("pd[0][%d]\taddr %X\t flags %X\n",
			    i, pgtable->addr, pgtable->flags);
	}

#if 0
	page_table_set(1023, (uint32_t)&page_directory, PG_RW|PG_US);

	load_page_directory(&page_directory);
#endif
}
