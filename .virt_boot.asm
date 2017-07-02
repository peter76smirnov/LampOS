global _loader
extern _main

ALIGN		equ 1<<0
MEMINFO		equ 1<<1
FLAGS		equ ALIGN | MEMINFO
MAGIC		equ 0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE	equ 0xC0000000
KERNEL_PAGE_NUMBER	equ (KERNEL_VIRTUAL_BASE >> 22)

.section .data
align 0x1000
PageDirectory:
	dd 0x00000083					; must be here
	times (KERNEL_PAGE_NUMBER - 1) dd 0		; before kernel
	dd 0x00000083					; kernel
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0	; after kernel

.section .text
align 4
MultBbootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

STACKSIZE equ 0x4000

loader equ (_loader - KERNEL_VIRTUAL_BASE)
global loader

_loader:
	mov ecx, (PageDirectory - KERNEL_VIRTUAL_BASE)
	mov cr3, ecx

	mov ecx, cr4
	or  ecx, 0x00000010				; 4 MiB size
	mov cr4, ecx

	lea ecx, [StartInHigherHalf]
	jmp ecx

StartInHigherHalf:
	mov dword [PageDirectory], 0
	invlpg [0]

	mov esp, stack+STACKSIZE
	push eax
	push ebx
	call _main
	hlt

.section .bss
align 32
stack:
	resb STACKSIZE
