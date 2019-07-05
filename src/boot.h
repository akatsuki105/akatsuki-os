// io
#define DEF_COLOR_BLACK 0x00
#define DEF_COLOR_WHITE 0x0F
void display_char(char c, unsigned char foreColor, unsigned char backColor, int x, int y);
void display_cursor(unsigned short pos);

// gdtidt
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1
#define AR_INTGATE32 0x008e
void init_gdtidt(void);
void init_pic();
void inthandler21(int *esp);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

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