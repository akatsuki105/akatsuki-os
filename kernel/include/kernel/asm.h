#ifndef _ASM_H_
#define _ASM_H_

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_hlt();
void io_stihlt();
void io_cli();
void io_sti();
void load_gdtr();
void load_idtr();
void as_timer_interrupt(void);
void as_keyboard_interrupt(void);
void as_software_interrupt(void);
int load_eflags(void);
void store_eflags(int eflags);

#endif _ASM_H_