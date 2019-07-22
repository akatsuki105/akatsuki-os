#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/gdtidt.h>
#include <kernel/asm.h>
#include <kernel/keyboard.h>
#include <kernel/fifo.h>
#include <kernel/multiboot.h>
#include <kernel/getmmap.h>

extern size_t pmstr_len;    //追加
size_t pmstr_len;
static size_t i;

int input_line(char* prompt_name, char* cmdline){
  	io_cli();
	if (fifo32_status(&fifo) == 0) {
		io_sti();
	} else {
		char c = fifo32_get(&fifo);
		io_sti();

		if (c == '\n') {
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

void shell(void){
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

void kernel_main(multiboot_info_t *mbt, uint32_t magic)
{
	terminal_initialize();
	init_gdt();
	init_idt();
	init_pic();
	init_key();
	fifo32_init(&fifo, 128, fifobuf);
	printf("Hello, Akatsuki World!\n");

	getmmap(mbt);

	shell();
}

