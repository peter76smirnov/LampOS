#include "kernel.h"
#include "stdio.h"
#include "vga.h"

int
kernel_main(uint32_t magic, void *mbinfo)
{
	vga_init();
//	segm_init();
//	paging_init();

	kprintf("Welcome\n%X\n", magic);
/*
	void *info = (void *)0x10000;
	char *boot_loader_name = (char *)info+64;
	kprintf("%s\n", *boot_loader_name);
*/
	return 0;
}
