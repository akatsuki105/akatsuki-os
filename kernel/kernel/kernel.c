#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/dsctbl.h>
#include <kernel/asm.h>
#include <kernel/keyboard.h>

extern key_buf kb;

void kernel_main(void) {
	terminal_initialize();
	init_gdt();
	init_idt();
	init_pic();
	init_key();
	printf("Hello, Akatsuki World!\n");
}

