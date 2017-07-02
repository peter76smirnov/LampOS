.global _loader

.set ALIGN,	1<<0
.set MEMINFO,	1<<1
.set FLAGS,	ALIGN | MEMINFO
.set MAGIC,	0x1BADB002
.set CHECKSUM,	-(MAGIC + FLAGS)

.set KERNEL_VIRT_BASE,	0xC0000000
.set KERNEL_PAGE_NUM,	(KERNEL_VIRT_BASE >> 22)

.section .data
.align 0x1000
page_dir:
	.long 0x00000083
	 .long 0
	.long 0x00000083
	 .long 0

.section .text
.align 4
multiboot_header:
	.long ALGIN
	.long FLAGS
	.long CHECKSUM

.set STACKSIZE, 0x4000

.set	loader, (_loader - KERNEL_VIRT_BASE)
.global loader

_loader:
	mov %ecx, (PageDirectory - KERNEL_VIRTUAL_BASE)
	mov %cr3, %ecx

	mov %ecx, %cr4
	or  %ecx, 0x00000010
	mov %cr4, %ecx

	lea %ecx, StartInHigherHalf
	jmp %ecx

StartInHigherHalf:
	movw  $PageDirectory, 0
	invlpg [0]

	mov %esp, $stack+STACKSIZE
	push %eax
	push %ebx
	call _main
	hlt

.section .bss
.align 32
stack:
	.skip STACKSIZE
