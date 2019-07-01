#include "multiboot.h"

void cstart(unsigned long magic, unsigned long addr)
{
    unsigned short *screen = (unsigned short *) 0xb8000;
    char *msg = "Starting Akatsuki OS!";
    char ch;
    char attr = 0x0f;

    while (ch = *msg++) {
        *screen++ = ((attr) << 8) | ch;
    }
    return;
}