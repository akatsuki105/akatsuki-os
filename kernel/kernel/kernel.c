#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdtidt.h>
#include <kernel/asm.h>
#include <kernel/keyboard.h>
#include <kernel/fifo.h>

extern key_buf kb;

void kernel_main(void)
{
	terminal_initialize();
	init_gdt();
	init_idt();
	init_pic();
	init_key();
	fifo32_init(&fifo, 32, fifobuf);
	printf("Hello, Akatsuki World!\n");

	char c[2];

	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			c[0] = fifo32_get(&fifo);
			asm volatile("sti");
			printf(c);
		}
	}
}

