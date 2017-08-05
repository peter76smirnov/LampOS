#include "kernel.h"
#include "stdio.h"
#include "vga.h"

#include "pag.h"

extern void load_idt();

int
kernel_main(uint32_t magic, void *mbinfo)
//kernel_main()
{
	vga_init();		/* set up VGA */
	segm_init();		/* set up GDT */
	load_idt();		/* set up IDT */
	pginit();		/* set up paging */

	kprintf("Welcome\n");
	kprintf("%X\n", magic);
	kprintf("%X\n", mbinfo);

//	mb_parseinfo(mbinfo);

	return 0;
}
