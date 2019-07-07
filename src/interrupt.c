#include "boot.h"
#include <stdint.h>
#include <stddef.h>

void interrupt_done(void)
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}

void keyboard_interrupt(void)
{
    outb(0x20, irq1);
    keyboard_input_int(getchar());
    interrupt_done();
}