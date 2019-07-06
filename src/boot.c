#include "multiboot.h"
#include "boot.h"
#include "keyboard.h"

void cstart(unsigned long magic, multiboot_info_t *info)
{   
    init_gdt();
    init_pic();
    init_idt();

    write_string("Hello Akatsuki OS!", 0, 3);
    keyboard_input_int();

    for (;;) {
		io_hlt();
	}
}
