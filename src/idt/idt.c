#include "idt.h"
#include "../config.h"
#include "../kernel.h"
#include "../io/io.h"
#include "../memory/memory.h"

struct idt_desc  idt_descriptors[TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void load_idt(struct idtr_desc *ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler() 
{
    print_str("keyboard pressed!!\n");
    out_b(0x20, 0x20);
}

void no_interrupt_handler() 
{
    out_b(0x20, 0x20);
}

void idt_zero_behavior() 
{
    print_str("divide by zero error!!\n");
}

void set_idt(int interrupt_nr, void *address) 
{
    struct idt_desc *desc = &idt_descriptors[interrupt_nr];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xee;
    desc->offset_2 = (uint32_t) address >> 16;
}

void init_idt() 
{
    mem_set(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base  = (uint32_t) idt_descriptors;

    for (int i = 0; i < TOTAL_INTERRUPTS; i++) 
    {
        set_idt(i, no_interrupt);
    }
    set_idt(0, idt_zero_behavior);
    set_idt(0x21, int21h);
    
    load_idt(&idtr_descriptor);
}
