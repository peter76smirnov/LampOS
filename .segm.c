#include <stdint.h>

#include "kernel.h"
#include "stdio.h"
#include "defs.h"

#define SEGM_CODE	0x9A
#define SEGM_DATA	0x92

#define SEGM_READ	(1 << 0)
#define SEGM_WRITE	(1 << 1)
#define SEGM_EXEC	(1 << 2)

#define PRIVL_KERNEL	0x0
#define PRIVL_USER	0x3

/* access_bit */
#define GDT_ACCESS	(1 << 0)	/* part of type */
#define GDT_RW		(1 << 1)	/* part of type */
#define GDT_DC		(1 << 2)	/* part of type */
#define GDT_EXEC	(1 << 3)	/* part of type */
#define GDT_DESC_TYPE	(1 << 4)
#define GDT_PRIVL_MASK	0x3		/* 56 -- privilege */
#define GDT_PRESENT	(1 << 7)

/* flags */
#define GDT_AVAIL	(1 << 0)
					/* 1 -- zero */
#define GDT_SIZE	(1 << 2)
#define GDT_GRAN	(1 << 3)

struct gdtr {
	uint16_t	size;
	uint32_t	offset;
}  __attribute__ ((packed));

struct gdt_entry {
	uint32_t	limit_low : 16;
	uint32_t	base_low : 24;
	uint8_t		access_byte : 8;
	uint32_t	limit_high : 4;
	uint8_t		flags : 4;
	uint32_t	base_high : 8;
} __attribute__ ((packed));

struct tss_struct {
	uint16_t	prev, res0;
	uint32_t	esp0;
	uint16_t	ss0, res1;
	uint32_t	esp1;
	uint16_t	ss1, res2;
	uint32_t	esp2;
	uint16_t	ss2, res3;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint16_t	es, res4;
	uint16_t	cs, res5;
	uint16_t	ss, res6;
	uint16_t	ds, res7;
	uint16_t	fs, res8;
	uint16_t	gs, res9;
	uint16_t	ldtr, res10;
	uint16_t	res11, iombase;
} __attribute__ ((packed));

static struct gdtr gdt_ptr;
static struct gdt_entry gdt_table[6];
static struct tss_struct tss;

static unsigned char interrupt_stack[4096];

extern void load_gdt(struct gdtr *gdt);

int
gdt_entry_set(unsigned index,
    uint32_t base, uint32_t limit, uint8_t privl, uint8_t flags)
{
	struct gdt_entry *gdtep;

	if (index >= NELEMS(gdt_table))
		return -1;

	gdtep = gdt_table + index;

	if (index == 0) {
		gdtep->access_byte = 0;
		gdtep->flags = 0;
		return 0;
	}

	gdtep->flags = GDT_SIZE;		/* mode32 */

	/* adjust granularity for segments >= 2^16 */
	if (limit >= 0xFFFF) {
		gdtep->flags |= GDT_GRAN;	/* in pages */
		gdtep->limit_low = (limit >> 12) & 0xFFFF;
		gdtep->limit_high = (limit >> 24) & 0xF;
	} else {
		gdtep->limit_low = limit & 0xFFFF;
		gdtep->limit_high = (limit >> 12) & 0xF;
	}

	/* set privilege level and present bit */
	gdtep->access_byte = (privl & GDT_PRIVL_MASK) << 5;
	gdtep->access_byte |= GDT_PRESENT;
	gdtep->access_byte |= GDT_DESC_TYPE;	/* type -- code/data */

	/*
	 * Write is never allowed for code segments.
	 * Data segments are always readable.
	 */
	if (flags & SEGM_EXEC) {
		gdtep->access_byte |= GDT_EXEC;
		if (flags & SEGM_READ)
			gdtep->access_byte |= GDT_RW;
	} else {
		if (flags & SEGM_WRITE)
			gdtep->access_byte |= GDT_RW;
	}

	kprintf("index: %u access_byte: %x flags: %x privl %x\n",
	    index, gdtep->access_byte, gdtep->flags, privl);

	return 0;
}

void
segm_init()
{
	gdt_ptr.size = sizeof gdt_table - 1;
	gdt_ptr.offset = (uint32_t)&gdt_table;

	/* NULL entry */
	gdt_entry_set(0, 0, 0, 0, 0);

	/* kernel code and data segments */
	gdt_entry_set(1, 0x0, 0xFFFFFFFF, PRIVL_KERNEL, SEGM_READ|SEGM_EXEC);
	gdt_entry_set(2, 0x0, 0xFFFFFFFF, PRIVL_KERNEL, SEGM_READ|SEGM_WRITE);

	/* user code and data segments */
	gdt_entry_set(3, 0x0, 0xFFFFFFFF, PRIVL_USER, SEGM_READ|SEGM_EXEC);
	gdt_entry_set(4, 0x0, 0xFFFFFFFF, PRIVL_USER, SEGM_READ|SEGM_WRITE);

	/* TSS */
	memset(&tss, 0, sizeof tss);
	tss.ss0 = KERNEL_DATA;
	tss.esp0 = (uint32_t)&interrupt_stack;
	tss.cs = KERNEL_CODE|PRIVL_USER;
	tss.ss = tss.es = tss.fs = tss.gs = KERNEL_DATA|PRIVL_USER;
	gdt_entry_set(5, (uint32_t)&tss, (uint32_t)&tss + sizeof tss, PRIVL_USER, SEGM_READ|SEGM_EXEC);

	load_gdt(&gdt_ptr);

	kputc('\n');
	kprintf("%X %X\n", gdt_table[0], (uint32_t)(&gdt_table[0]+4));
	kprintf("%X %X\n", gdt_table[1], (uint32_t)(&gdt_table[1]+4));
	kprintf("%X %X\n", gdt_table[2], (uint32_t)(&gdt_table[2]+4));
	kprintf("%X %X\n", gdt_table[3], (uint32_t)(&gdt_table[3]+4));
	kprintf("%X %X\n", gdt_table[4], (uint32_t)(&gdt_table[4]+4));
	kprintf("%X %X\n", gdt_table[5], (uint32_t)(&gdt_table[5]+4));
}

