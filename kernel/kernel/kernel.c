#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/dsctbl.h>

void kernel_main(void) {
	terminal_initialize();
	init_gdt();
	init_idt();
	printf("Hello, Akatsuki World!\n");
}
