#include "kernel/asm.h"
#include "kernel/dsctbl.h"
#include "kernel/interrupt.h"
#include "kernel/keyboard.h"

void interrupt_done(void){
  outb(MASTER_PIC_CMD_STAT, PIC_EOI);
  outb(SLAVE_PIC_CMD_STAT, PIC_EOI);
}
 
 
void keyboard_interrupt(void){
  outb(MASTER_PIC_CMD_STAT, irq1);
  keyboard_input_int(getchar());
  interrupt_done();
}
