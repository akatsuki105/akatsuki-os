#include "multiboot.h"
#include "boot.h"
#include "keyboard.h"

void cstart(unsigned long magic, multiboot_info_t *info)
{   
    write_string("Hello Akatsuki OS!", 0, 0);
    keyboard_input_int();

    for (;;) {
		io_hlt();
	}
}
