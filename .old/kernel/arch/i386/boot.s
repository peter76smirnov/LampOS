.set ALIGN,	1<<0
.set MEMINFO,	1<<1
.set FLAGS,	ALIGN | MEMINFO
.set MAGIC,	0x1BADB002
.set CHECKSUM,	-(MAGIC + FLAGS)

.section multiboot
.long	MAGIC
.long	FLAGS
.long	CHECKSUM

.section .bss
.align	16	# stack must be 16-byte aligned
stack_bottom:	# stack grows downwards on x86
.skip	16384	# 16 KiB
stack_top:

.section .text
.global	_start
.type	_start,	@function
_start:
	# 32-bit protected mode
	# interrupts disabled
	# paging disabled

	mov $stack_top,	%esp	# set up stack

	# GDT should be loaded here
	# paging should be enabled here

	# enter high-level kernel
	call kernel_main

	# put computer into infinite loop
	cli	# disable interrupts
1:	hlt	# wait for the next interrupt
	jmp 1b	# jump to hlt if it ever wakes up

.size	_start,	. - _start	# set size of _start symbol
