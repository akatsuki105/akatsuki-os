unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_hlt();
void io_stihlt();
void io_cli();
void io_sti();
void load_gdtr();
