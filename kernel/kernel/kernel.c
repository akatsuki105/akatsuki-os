#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdtidt.h>
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

	/* 以下から変更 */
	char c[2];
	kb.len = 0;
	kb.write = 0;
	kb.read = 0;

	for (;;) {
		asm volatile("cli");
		if (kb.len == 0) {
			asm volatile("sti");
		} else {
			c[0] = kb.pdata[kb.read];
			--kb.len;
			++kb.read;
			if (kb.read == 128) { kb.read = 0; }
			asm volatile("sti");
			printf(c);
		}
	}
}

