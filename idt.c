#include <stdint.h>

#include "kernel.h"
#include "stdio.h"

struct idtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

struct idt_entry {
	uint16_t	offset_low;	/* offset 0..15 */
	uint16_t	selector;	/* code segment selector in GDT/LDT */
	uint8_t		zero;		/* not used */
	uint8_t		type_byte;	/* type and attributes */
	uint16_t	offset_high;	/* offset 16..31 */
} __attribute__ ((packed));

static struct idtr idt_ptr;
static struct idt_entry idt_table[256];

extern void load_idt(struct idtr *);

void interrupt_handler()
{
	__asm__ __volatile__ ("pusha");

	kprintf("it works!\n");

	__asm__ __volatile__ ("popa; leave; iret");
}

void
idt_init()
{
	idt_ptr.limit = sizeof idt_table - 1;
	idt_ptr.base = (uint32_t)&idt_table;

	memset(idt_table, 0, sizeof idt_table);

	idt_table[50].offset_low =	(uint32_t)interrupt_handler;
	idt_table[50].selector =	0x8;
	idt_table[50].zero =		0x0;
	idt_table[50].type_byte =	0x8E;
	idt_table[50].offset_high =	(uint32_t)interrupt_handler >> 16;

	load_idt(&idt_ptr);

	kprintf("IDT\t%X\t%X\n\n", idt_ptr.limit, idt_ptr.base);
}

