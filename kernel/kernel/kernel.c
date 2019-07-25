#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/gdtidt.h>
#include <kernel/asm.h>
#include <kernel/keyboard.h>
#include <kernel/fifo.h>
#include <kernel/multiboot.h>
#include <kernel/getmmap.h>
#include <kernel/memory.h>
#include <kernel/timer.h>
#include <kernel/mtask.h>

size_t pmstr_len;
static size_t i;

int input_line(char* prompt_name, char* cmdline){
  	io_cli();
	if (fifo32_status(&kernelfifo) == 0) {
		io_sti();
	} else {
		char c = fifo32_get(&kernelfifo);
		io_sti();

		if (c == '\n') {
			farjmp(0, 4*8);
			cmdline[i] = '\0';
			return 0;
		} else if (c == '\b') {
			cmdline[i] == '\0';
		if (i > 0) { --i; }
		} else {
			cmdline[i++] = c;
		}
		printf("%c", c);
	}
	return -1;
}

int execute_cmd(char *cmdline)
{
	if (strncmp(cmdline, "echo ", 5) == 0) {
		char *operand = cmdline + 5;
		printf("\n%s", operand);
		return 0;
	}
	return -1;
}

void shell(void)
{
	int result = -1;
	char cmdline[1024];
	char *prompt_name = "akatsuki";
	pmstr_len = strlen(prompt_name);
	printf("\n%s> ", prompt_name);
	i = 0;

	for (;;) {
		if ((result = input_line(prompt_name, cmdline)) != -1) {
			if (i) {
				if ((execute_cmd(cmdline)) == -1) {
					printf("\nCommand not found!");
				}
			}
			printf("\n%s> ", prompt_name);
			result = -1;
			i = 0;
		}
	}
}

void task_b_main(void)
{
	for (;;) {
		printf("0");
	}
}

void kernel_main(multiboot_info_t *mbt, uint32_t magic)
{
	// init hardware
	init_terminal();
	init_gdt();
	init_idt();
	init_pic();
	init_key();
	init_pit();

	// init fifo
	init_fifo32(&kernelfifo, 128, kernelfifo_buf);

	// memory manage (64MB)
	memory_manager *memman = (memory_manager *)MEMMAN_ADDR;
	init_memman(memman);
	memman_free(memman, 0x100000, 0x3ef0000);

	// init multitask
	struct TSS32 tss_a, tss_b;
	tss_a.ldtr = 0;
	tss_a.iomap = 0x40000000;
	tss_b.ldtr = 0;
	tss_b.iomap = 0x40000000;
	set_segment_desc(3, (int)&tss_a, 103, 0x89, 0x00);
	set_segment_desc(4, (int)&tss_b, 103, 0x89, 0x00);
	load_tr(3*8);
	int task_b_esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024;
	tss_b.eip = (int) &task_b_main;
	tss_b.eflags = 0x00000202; /* IF = 1; */
	tss_b.eax = 0;
	tss_b.ecx = 0;
	tss_b.edx = 0;
	tss_b.ebx = 0;
	tss_b.esp = task_b_esp;
	tss_b.ebp = 0;
	tss_b.esi = 0;
	tss_b.edi = 0;
	tss_b.es = 2 * 8;
	tss_b.cs = 1 * 8;
	tss_b.ss = 2 * 8;
	tss_b.ds = 2 * 8;
	tss_b.fs = 2 * 8;
	tss_b.gs = 2 * 8;

	printf("Hello, Akatsuki OS!\n");

	shell();
}

