#include "multiboot.h"
#include "boot.h"

char keytable[0x80] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
};

void cstart(unsigned long magic, multiboot_info_t *info)
{
    int i;
    char *s;
    // hello msg
    char *hello_msg = "Starting Akatsuki OS!";
    // memory msg
    char *memory_msg = "Memory size: ";
    int memory_size = (int)info->mem_upper;

    for (i = 0; *hello_msg != 0; i++) {
        display_char(*hello_msg++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 0);
    }

    for (i = 0; *memory_msg != 0; i++) {
        display_char(*memory_msg++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 1);
    }
    sprintf(s, "%d", memory_size);
    for (; *s != 0; i++) {
        display_char(*s++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 1);
    }

    for (;;) {
		io_hlt();
	}
}
