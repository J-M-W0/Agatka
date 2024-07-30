#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

extern void enable_interrupt();
extern void disable_interrupt();

struct idt_desc 
{
    uint16_t offset_1;  // offset bits 0-15
    uint16_t selector;  // selector in our GDT
    uint8_t  zero;      // does nothing, just set to zero
    uint8_t  type_attr; // descriptor type and attributes
    uint16_t offset_2;  // offset bits 16-31
} __attribute__((packed));

struct idtr_desc
{
    uint16_t limit; // size of descriptor table - 1
    uint32_t base;  // base address of the IDT
} __attribute__((packed));

void init_idt();

#endif // __IDT_H
