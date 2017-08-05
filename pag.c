#include <stdint.h>

#include "kernel.h"
#include "stdio.h"
#include "pag.h"

#define	VADDR(di, ti, off)	((di<<22) + (ti<<12) + off)

extern void load_pgdir(void *);
extern void _pgupdate();

void
pginit()
{
	int i;

	/* Identity paging first 4M */
	pgdirectory[0] = (uint32_t)pgtable | 0x3;
	for (i = 0; i < 1024; i++) {
		pgtable[0][i] = (i * 0x1000) | 0x3;
	}

	/* Self-referencing dir entry */
	pgdirectory[1023] = (uint32_t)pgdirectory | 0x3;

	kprintf("page directory address\t%X\n\n", pgdirectory);
	kprintf("enabling paging...\n");

	load_pgdir(&pgdirectory);

	kprintf("Hello, paging world!\n\n");

	kprintf("page table address %X\n", pgtable);
	kprintf("allocated\t%X\n", pgalloc());
	kprintf("allocated\t%X\n", pgalloc());
	pgfree(0x105004);
	kprintf("allocated\t%X\n", pgalloc());
	pgfree(0x105000);
	kprintf("allocated\t%X\n", pgalloc());
}

paddr_t
pgalloc()
{
	uint32_t **table;
	int i;

//	table = (uint32_t **)(pgdirectory[0] & 0xFFFFF000);

//	kprintf("\t\t%X\n", pgdirectory[0] & 0xFFFFF000);

//	kprintf("\t%X\n", table);

	for (i = 0; i < 1024; i++) {
		int j;

		if (!(pgdirectory[i] & PG_PRESENT))
			continue;

		table = (uint32_t **)(pgdirectory[i] & 0xFFFFF000);

		kprintf("\t%X\n", *table[0]);

		for (j = 0; j < 1024; j++) {
			if (!(*table[j] & PG_PRESENT)) {
				*table[j] |= PG_PRESENT;
				_pgupdate();

//				return (paddr_t)table[j];
				return 0;
			}
		}
	}

	return 0;
}

void
pgfree(paddr_t addr)
{
	*(paddr_t *)addr &= ~PG_PRESENT;
	kprintf("freed\t%X\n", addr);
	_pgupdate();
}

