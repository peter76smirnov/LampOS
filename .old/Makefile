KERNEL_LD = link-kernel.ld
GRUB_CFG = grub.cfg
CC = /home/peter76smirnov/opt/cross/bin/i686-elf-gcc
CFLAGS = -O2 -Wall -Wextra
sources = kernel.c
objects = $(sources:.c=.o)

all: boot.iso

%.o: %.c
	$(CC) $(CFLAGS) -c -T $(KERNEL_LD) -nostdlib -ffreestanding -o $@ $<

boot.bin: boot.s $(objects)
	$(CC) $(CFLAGS) -T $(KERNEL_LD) -nostdlib -ffreestanding -o $@ $^ -lgcc

boot.iso: boot.bin
	mkdir -p iso/boot/grub
	cp $(GRUB_CFG) iso/boot/grub/grub.cfg
	cp boot.bin iso/boot/boot.bin
	grub-mkrescue -o boot.iso iso/

clean:
	-rm -rf boot.bin boot.iso
	-rm -rf $(objects)

.PHONY: clean
