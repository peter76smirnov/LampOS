#define PAGE_ALIGN	1<<0
#define MEMORY_INFO	1<<1
#define HEADER_MAGIC	0x1BADB002
#define HEADER_FLAGS	PAGE_ALIGN | MEMORY_INFO
#define CHECKSUM	-(HEADER_MAGIC + HEADER_FLAGS)

.section .multiboot
.long HEADER_MAGIC
.long HEADER_FLAGS
.long CHECKSUM

.section .bss
.align	0x10
stack_bottom:
.skip	0x4000
stack_top:

.section .text

.global	_start
.global cli
.global sti
.global load_gdt
.global load_idt
.global load_pgdir

.type	_start,	@function
_start:
	# 32-bit protected mode
	# interrupts disabled
	# paging disabled

	# set up interrupt descriptor table

	#set up stack
	mov	$stack_top, %esp

	# enter high-level kernel
	push	%ebx
	push	%eax
	call	kernel_main

	addl	$8, %esp

	# put computer into infinite loop
	cli		# disable interrupts
	hlt		# halt CPU (wait for the next interrupt)
.Loop:
	jmp	.Loop	# jump in endless loop if it somehow continues exexution

.size	_start,	. - _start	# set size of _start symbol

cli:
	pushfl
	cli
	popl	%eax
	ret

sti:
	sti
	ret

load_gdt:
	movl	0x4(%esp), %eax
	lgdt	(%eax)
	ljmp	$(0x08), $(.Lfinish)
.Lfinish:
	mov	$(0x10), %eax
	mov	%eax, %ds
	mov	%eax, %es
	mov	%eax, %gs
	mov	%eax, %fs
	mov	%eax, %ss
	ret


.align	0x4
idt:
.skip	0x188	# 49 * 8
idt_49:
	.word	0x0	# offset low
	.word	0x8
	.byte	0x0
	.byte	0x8E
	.word	0x0	# offset high
.skip	0x670	# 206 * 8

idtr:
	.word	256*8 - 1
	.long	(idt)

fmt:
.asciz "IDT address\t%X\n\n"

int_handler:
	pushl	$idt
	pushl	$fmt
	call	kprintf
	add	$8, %esp
	iret

load_idt:
	lidt	(idtr)

#	mov	int_handler, idt_49
#	mov	int_handler>>16, idt_49 + 6

	mov	$(idt_49), %eax
	mov	$(int_handler), %ebx
	mov	%bx, (%eax)
	shr	$16, %ebx
	mov	%bx, 0x6(%eax)

	int	$49
	ret

load_pgdir:
	movl	0x4(%esp), %eax
	mov	%eax, %cr3

	mov	%cr0, %eax
	or	$0x80000000, %eax
	mov	%eax, %cr0
	jmp	.finish
.finish:
	ret

