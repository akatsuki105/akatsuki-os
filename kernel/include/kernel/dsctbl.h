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
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_LDT			0x0082
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void init_gdt(void);
void init_idt(void);

#define MASTER_PIC_CMD_STAT  0x20
#define MASTER_PIC_MASK_DATA 0x21
#define SLAVE_PIC_CMD_STAT   0xA0
#define SLAVE_PIC_MASK_DATA  0xa1
#define WRITE_ICW1           0x11
#define WRITE_ICW2_M         0x20
#define WRITE_ICW2_S         0x28
#define WRITE_ICW3_M         0x04
#define WRITE_ICW3_S         0x02
#define WRITE_ICW4_X86MODE   0x01
#define CLEAR_MASK           0xff
#define PIC_MASK_IRQ0        0x01
#define PIC_MASK_IRQ1        0x02
#define PIC_MASK_IRQ2        0x04
#define PIC_MASK_IRQ3        0x08
#define PIC_MASK_IRQ4        0x10
#define PIC_MASK_IRQ5        0x20
#define PIC_MASK_IRQ6        0x40
#define PIC_MASK_IRQ7        0x80
#define PIC_MASK_IRQA        0x00
#define PIC_EOI              0x20
 
void init_pic(void);
