#include <stdio.h>

#include <kernel/asm.h>
#include <kernel/dsctbl.h>

void init_gdt(void)
{
	printf("setup gdt...");
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	int i;

	for (i = 0; i <= LIMIT_GDT / 8; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff,   0x00000000, AR_CODE32_ER);
	set_segmdesc(gdt + 2, 0xffffffff, 0x00000000, AR_DATA32_RW);
	load_gdtr(LIMIT_GDT, ADR_GDT);
	printf("OK!\n");
	return;
}

void init_idt(void)
{
	printf("setup idt...");
	struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *) ADR_IDT;
	int i;

	for (i = 0; i <= LIMIT_IDT / 8; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);

	set_gatedesc(idt + 0x21, (int)as_keyboard_interrupt, 1 * 8, 0x8e);

	printf("OK!\n");
	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}

void init_pic(void){
  printf("setup PIC...");
 
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
 
  printf("OK!\n");
}
