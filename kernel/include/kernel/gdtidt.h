#ifndef _GDTIDT_H_
#define _GDTIDT_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" void load_gdtr(uint32_t);
extern "C" void load_idtr(uint32_t);
#else
extern void load_gdtr(uint32_t);
extern void load_idtr(uint32_t);
#endif

// gdt
typedef struct{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t s_access;
  uint8_t limit_high;
  uint8_t base_high;
}__attribute__((packed)) gdt_desc;

typedef struct{
  uint16_t gdt_size;
  uint32_t base;
}__attribute__((packed)) gdtr;

#define GDT_LEN 8192
gdt_desc gdt_entries[GDT_LEN];

void init_gdt();
void set_segment_desc(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

// idt
typedef struct{
  uint16_t offset_low;
  uint16_t selector;
  uint8_t dw_count;
  uint8_t s_access;
  uint16_t offset_high;
}__attribute__((packed)) gate_desc;

typedef struct{
  uint16_t idt_size;
  uint32_t base;
}__attribute__((packed)) idtr;

#define LIDT      0x000007ff
#define IDT_LEN   256
gate_desc idt_entries[IDT_LEN];

void init_idt(void);
void set_gate_desc(uint32_t, uint32_t, uint32_t, uint8_t);

// pic
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

#endif _GDTIDT_H_