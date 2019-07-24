#include <kernel/asm.h>
#include <kernel/gdtidt.h>

void init_gdt()
{
	gdtr gdt;
	// clear gdt
	for (size_t i = 0; i < GDT_LEN; i++) {
		set_segment_desc(i, 0, 0, 0, 0);
	}
	// init segment
	// id,  base, limit, s_access, granularity
	set_segment_desc(1, 0, 0xffffffff, 0x9a, 0xcf);
	set_segment_desc(2, 0, 0xffffffff, 0x92, 0xcf);
  	set_segment_desc(3, 0, 0xffffffff, 0xfa, 0xcf);
  	set_segment_desc(4, 0, 0xffffffff, 0xf2, 0xcf);

	gdt.gdt_size = GDT_LEN * sizeof(gdt_desc) - 1;
	gdt.base = (uint32_t)gdt_entries;
	load_gdtr((uint32_t)(&gdt));
}

void init_idt(void)
{
	idtr idt;
	for (size_t i = 0; i < IDT_LEN; i++) {
		set_gate_desc(i, 0, 0, 0);
	}

	set_gate_desc(0x20, (uint32_t)as_timer_interrupt, 1*8, 0x8e);
	set_gate_desc(0x21, (uint32_t)as_keyboard_interrupt, 1*8, 0x8e);
	set_gate_desc(0x80, (uint32_t)as_software_interrupt, 1*8, 0x8f);
	idt.idt_size = IDT_LEN * sizeof(gate_desc) -1;
	idt.base = (uint32_t)idt_entries;
	load_idtr((uint32_t)&(idt));
}

void set_segment_desc(uint32_t index, uint32_t base, uint32_t limit, uint8_t s_access, uint8_t gran)
{
	gdt_desc * sd = &gdt_entries[index];
	sd -> base_high = (base >> 24 & 0xff);
	sd -> base_low = base & 0xffff;
	sd -> base_mid = (base >> 16) & 0xff;
	sd -> s_access = s_access;
	sd -> limit_low = limit & 0xffff;
	sd -> limit_high = ((limit >> 16) & 0x0f) | (gran & 0xf0);
}

void set_gate_desc(uint32_t index, uint32_t offset, uint32_t selector, uint8_t ar)
{
	gate_desc *gd = &idt_entries[index];
	gd -> offset_low = offset & 0xffff;
	gd -> offset_high = (offset >> 16) & 0xffff;
	gd -> dw_count = 0;
	gd -> selector = selector;
	gd -> s_access = ar | 0x60;
}

void init_pic(void)
{
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

  outb(MASTER_PIC_MASK_DATA, 0xf8); // PIT & IRQ1 & IRQ2
  outb(SLAVE_PIC_MASK_DATA, 0xef);
}
