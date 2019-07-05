// io
#define DEF_COLOR_BLACK 0x00
#define DEF_COLOR_WHITE 0x0F
void display_char(char c, unsigned char foreColor, unsigned char backColor, int x, int y);
void display_cursor(unsigned short pos);

/* nasm.asm */
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_hlt();
void io_stihlt();
void io_cli();
void io_sti();
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);

/* fifo.c */
struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

/* stdlib.c */
void kmemset(void *start, unsigned char c, int size);
void sprintf(char *str, char *fmt, ...);