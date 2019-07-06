// io
void write_string(char *target_string, int pos_x, int pos_y);
void display_char(char c, int x, int y);
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
int strlen(const char *s);
void sprintf(char *str, char *fmt, ...);

/* gdtidt.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
void init_gdtidt(void);
