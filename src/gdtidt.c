#include "boot.h"

// gdt・idt
#define ADR_IDT 0x0026f800
#define LIMIT_IDT 0x000007ff
#define ADR_GDT 0x00270000
#define LIMIT_GDT 0x0000ffff
#define ADR_BOTPAK 0x00280000
#define LIMIT_BOTPAK 0x0007ffff
#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_INTGATE32 0x008e

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff)
    {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
    return;
}

void init_gdt(void)
{
    char msg[] = "setup GDT...";
    write_string(msg, 0, 0);
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
    int i;

    for (i = 0; i <= LIMIT_GDT / 8; i++)
    {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
    load_gdtr(LIMIT_GDT, ADR_GDT);

    write_string("OK!", strlen(msg) + 1, 0);
    return;
}

void init_idt(void)
{
    char msg[] = "setup IDT...";
    write_string(msg, 0, 2);
    struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *)ADR_IDT;
    int i;

    for (i = 0; i <= LIMIT_IDT / 8; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }

    set_gatedesc(idt + 0x21, (int) asm_keyboard_interrupt, 2 * 8, 0x8e);

    load_idtr(LIMIT_IDT, ADR_IDT);

    write_string("OK!", strlen(msg) + 1, 2);
    return;
}

void init_pic(void)
{
    char msg[] = "setup PIC...";
    write_string(msg, 0, 1);

    outb(MASTER_PIC_MASK_DATA, CLEAR_MASK);
    outb(SLAVE_PIC_MASK_DATA, CLEAR_MASK);

    outb(MASTER_PIC_CMD_STAT, WRITE_ICW1);
    outb(SLAVE_PIC_CMD_STAT, WRITE_ICW1);

    outb(MASTER_PIC_MASK_DATA, WRITE_ICW2_M);
    outb(SLAVE_PIC_MASK_DATA, WRITE_ICW2_S);

    outb(MASTER_PIC_MASK_DATA, WRITE_ICW3_M);
    outb(SLAVE_PIC_MASK_DATA, WRITE_ICW3_S);

    outb(MASTER_PIC_MASK_DATA, WRITE_ICW4_X86MODE);
    outb(SLAVE_PIC_MASK_DATA, WRITE_ICW4_X86MODE);

    outb(MASTER_PIC_MASK_DATA, 0xf9);
    outb(SLAVE_PIC_MASK_DATA, 0xef);

    write_string("OK!", strlen(msg) + 1, 1);
}
