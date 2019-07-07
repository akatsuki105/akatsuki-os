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
void asm_keyboard_interrupt(void);

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
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdt(void);
void init_pic(void);
void init_idt(void);

/* keyboard.c */
void init_keyboard(void);
char ps2_kerboard_init(void);
void keyboard_input_int(char scan_code);
char enable_keyboard(void);
char getscode(void);
char getscodeset(void);
char getchar(void);
void change_codeset(char set);
void change_trate_delay(char set);

/* interrupt.c */
void interrupt_done(void);
void keyboard_interrupt(void);
#define irq1 0x61