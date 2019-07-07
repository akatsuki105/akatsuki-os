#include "multiboot.h"
#include "boot.h"

extern struct FIFO32 fifo;

void cstart(unsigned long magic, multiboot_info_t *info)
{   
    int key_buf[30];
    char c[2];
    fifo32_init(&fifo, 30, key_buf);
    init_gdt();
    init_pic();
    init_idt();
    init_keyboard();

    write_string("Hello Akatsuki OS!", 0, 5);

    for (;;) {
		io_cli();
        if (fifo32_status(&fifo) == 0) {
            io_sti();
        } else {
            c[0] = (char)fifo32_get(&fifo);
            io_sti();
            write_string(c, 0, 7);
        }
	}
}
