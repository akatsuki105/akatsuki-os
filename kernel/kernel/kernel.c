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
#include <kernel/fs.h>

size_t pmstr_len;
static size_t i;

int input_line(char* prompt_name, char* cmdline){
	char c = fifo32_get(&shellfifo);
	io_sti();
	if (c == '\n') {
		cmdline[i] = '\0';
		return 0;
	} else if (c == '\b') {
		cmdline[i] = '\0';
		if (i > 0) {
			printf("%c", c);
			--i;
		}
	} else if (c == '\t') {
		cmdline[i] = '    ';
		i += 4;
		printf("%c", c);
	} else {
		cmdline[i++] = c;
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
	} else if (strcmp(cmdline, "clear") == 0) {
		init_terminal();
		return 0;
	} else if (strcmp(cmdline, "reboot") == 0) {
		outb(0x64, 0xFE);
		return 0;
	} else if (strcmp(cmdline, "free") == 0) {
		memory_manager *memman = (memory_manager *)MEMMAN_ADDR;
		int memory_free = memman_total(memman) / (1024 * 1024);
		printf("\nmemory: %dMB", memory_free);
		return 0;
	} else if (strncmp(cmdline, "touch ", 6) == 0) {
		char *operand = cmdline + 6;
		file_write(operand, "", 0);
		return 0;
	} else if (strncmp(cmdline, "mkdir ", 6) == 0) {
		char *operand = cmdline + 6;
		file_write(operand, "", 1);
		return 0;
	} else if (strcmp(cmdline, "ls") == 0) {
		lsdir();
		return 0;
	} else if (strncmp(cmdline, "cd ", 3) == 0) {
		char *operand = cmdline + 3;
		chdir(operand);
		return 0;
	}
	return -1;
}

void shell(struct TASK *shell_task)
{
	int result = -1;
	char cmdline[1024];
	char *prompt_name = "akatsuki";
	pmstr_len = strlen(prompt_name);
	printf("\n%s> ", prompt_name);
	i = 0;

	for (;;) {
		io_cli();
		if (fifo32_status(&shellfifo) == 0) {
			task_sleep(shell_task);
			io_sti();
		} else {
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
	init_fifo32(&kernelfifo, 128, kernelfifo_buf, 0);
	init_fifo32(&shellfifo, 128, shellfifo_buf, 0);

	// memory manage (64MB)
	memory_manager *memman = (memory_manager *)MEMMAN_ADDR;
	init_memman(memman);
	memman_free(memman, 0x100000, 0xdfef0000);
	memman_free(memman, 0x100000000, 0x20000000);

	// init multitask
	struct TASK *kernel_task = init_multitask(memman);
	kernelfifo.task = kernel_task;
	task_run(kernel_task, 1, 2);

	// init filesystem
	init_fs();

	// init shell
	struct TASK *shell_task = task_alloc();
	shellfifo.task = shell_task;
	shell_task->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 8;
	shell_task->tss.eip = (int) &shell;
	shell_task->tss.es = 2 * 8;
	shell_task->tss.cs = 1 * 8;
	shell_task->tss.ss = 2 * 8;
	shell_task->tss.ds = 2 * 8;
	shell_task->tss.fs = 2 * 8;
	shell_task->tss.gs = 2 * 8;
	*((int *) (shell_task->tss.esp + 4)) = (int) shell_task;
	task_run(shell_task, 2, 2);

	printf("Hello, Akatsuki OS!\n");

	for (;;) {
		io_cli();
		if (fifo32_status(&kernelfifo) == 0) {
			task_sleep(kernel_task);
			io_sti();
		} else {
			int i = fifo32_get(&kernelfifo);
			io_sti();
			fifo32_put(&shellfifo, i);
		}
	}
}

