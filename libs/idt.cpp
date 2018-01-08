#include <stddef.h>
#include <stdint.h>
#include "idt.h"
#include "terminal.h"

#define IDT_ENTRY_COUNT 256

IDT_Class IDT;

uint8_t _IDT[IDT_ENTRY_COUNT * 8];

void lidt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

void encodeIdtEntry(uint8_t *target, uint32_t offset, uint16_t selector, uint8_t type)
{
    // Encode the offset
    target[0] = offset & 0xFF;
    target[1] = (offset >> 8) & 0xFF;
    target[6] = (offset >> 16) & 0xFF;
    target[7] = (offset >> 24) & 0xFF;
 
    // Encode the selector 
    target[2] = selector & 0xFF;
    target[3] = (selector >> 8) & 0xFF;

    // Encode the Zero
    target[4] = 0x00;
 
    // And... Type
    target[5] = type;
}

void IDT_Class::load() {

    for (int i = 0; i < 256; i++) {
        encodeIdtEntry(&_IDT[i * 8], (uint32_t)&ASM_ISR_0, 0x08, 0x8E);
    }

    encodeIdtEntry(&_IDT[8 * 0], (uint32_t)&_isr0, 0x08, 0x8E); // Exceptions
    encodeIdtEntry(&_IDT[8 * 1], (uint32_t)&_isr1, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 2], (uint32_t)&_isr2, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 3], (uint32_t)&_isr3, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 4], (uint32_t)&_isr4, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 5], (uint32_t)&_isr5, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 6], (uint32_t)&_isr6, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 7], (uint32_t)&_isr7, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 8], (uint32_t)&_isr8, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 9], (uint32_t)&_isr9, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 10], (uint32_t)&_isr10, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 11], (uint32_t)&_isr11, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 12], (uint32_t)&_isr12, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 13], (uint32_t)&_isr13, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 14], (uint32_t)&_isr14, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 15], (uint32_t)&_isr15, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 16], (uint32_t)&_isr16, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 17], (uint32_t)&_isr17, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 18], (uint32_t)&_isr18, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 19], (uint32_t)&_isr19, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 20], (uint32_t)&_isr20, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 21], (uint32_t)&_isr21, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 22], (uint32_t)&_isr22, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 23], (uint32_t)&_isr23, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 24], (uint32_t)&_isr24, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 25], (uint32_t)&_isr25, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 26], (uint32_t)&_isr26, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 27], (uint32_t)&_isr27, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 28], (uint32_t)&_isr28, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 29], (uint32_t)&_isr29, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 30], (uint32_t)&_isr30, 0x08, 0x8E);
    encodeIdtEntry(&_IDT[8 * 31], (uint32_t)&_isr31, 0x08, 0x8E);

    encodeIdtEntry(&_IDT[8 * 33], (uint32_t)&ASM_ISR_KBD, 0x08, 0x8E); // Keyboard

    lidt(&_IDT, 256 * 8);
}