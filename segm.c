#include <stdint.h>

#include "kernel.h"
#include "stdio.h"
#include "defs.h"

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

struct gdt_entry {
	uint16_t	limit_0;
	uint16_t	base_0;
	uint8_t		base_1;
	uint8_t		acs_byte;
	uint8_t		gran_byte;
	uint8_t		base_2;
} __attribute__ ((packed));

struct tss_struct {
	uint16_t	link, res0;
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
	uint16_t	ldt, res10;
	uint16_t	res11, iombase;	/* res11[0] debug trap bit */
} __attribute__ ((packed));

static struct gdtr gdt_ptr;
static struct gdt_entry gdt_table[6];
static struct tss_struct tss;

static unsigned char interrupt_stack[4096];

extern void load_gdt(struct gdtr *);

void
segm_init()
{
	uint32_t tss_base = (uint32_t)&tss;
	uint32_t tss_limit = tss_base + sizeof tss;

	gdt_ptr.limit = sizeof gdt_table - 1;
	gdt_ptr.base = gdt_table;

	/*  NULL descriptor */
	memset(gdt_table, 0, sizeof (struct gdt_entry));
	/*  Kernel code */
	gdt_table[1].limit_0 =	0xFFFF;
	gdt_table[1].base_0 =	0x0;
	gdt_table[1].base_1 =	0x0;
	gdt_table[1].acs_byte =	0x9A;
	gdt_table[1].gran_byte =0xFC;
	gdt_table[1].base_2 =	0x0;
	/*  Kernel data */
	gdt_table[2].limit_0 =	0xFFFF;
	gdt_table[2].base_0 =	0x0;
	gdt_table[2].base_1 =	0x0;
	gdt_table[2].acs_byte =	0x92;
	gdt_table[2].gran_byte =0xFC;
	gdt_table[2].base_2 =	0x0;
	/*  User code */
	gdt_table[3].limit_0 =	0xFFFF;
	gdt_table[3].base_0 =	0x0;
	gdt_table[3].base_1 =	0x0;
	gdt_table[3].acs_byte =	0xFA;
	gdt_table[3].gran_byte =0xFC;
	gdt_table[3].base_2 =	0x0;
	/*  User data */
	gdt_table[4].limit_0 =	0xFFFF;
	gdt_table[4].base_0 =	0x0;
	gdt_table[4].base_1 =	0x0;
	gdt_table[4].acs_byte =	0xF2;
	gdt_table[4].gran_byte =0xFC;
	gdt_table[4].base_2 =	0x0;

	/* TSS */
	/* 0x8	code selector */
	/* 0x10	data selector */
	memset(&tss, 0, sizeof tss);
	tss.ss0 = 0x10;
	tss.esp0 = (uint32_t)&interrupt_stack;
	tss.cs = 0x8 | 0x3;
	tss.ss = tss.es = tss.fs = tss.gs = 0x10 | 0x3;

	gdt_table[5].limit_0 =	tss_limit;
	gdt_table[5].base_0 =	tss_base;
	gdt_table[5].base_1 =	tss_base >> 16;
	gdt_table[5].acs_byte =	0xFA;
	gdt_table[5].gran_byte =tss_limit>>16 | 0xC;
	gdt_table[5].base_2 =	tss_base >> 24;

	load_gdt(&gdt_ptr);

	kprintf("GDT address\t%x\n\n", gdt_table);
}

