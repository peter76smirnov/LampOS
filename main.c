#include "kernel.h"
#include "stdio.h"
#include "vga.h"

int
kernel_main()
{
	vga_init();
	segm_init();
	paging_init();
/*
	vga_puts("Hello, kernel world!\n");
	vga_puts("\ttest\ttest\n");
	vga_puts("D\bC if fun\n");
*/
	kprintf("Hello, paging world!\n");
	kprintf("\ttest\ttest\n");
	kprintf("D\bC is fun! (no)");

	return 0;
}
