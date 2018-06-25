#include <stddef.h>
#include <stdint.h>
#include <idt.h>

#define IDT_ENTRY_COUNT 256

#define INT_GATE        0x8E
#define TRAP_GATE       0xEF

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

void encodeIdtEntry(uint8_t index, uint32_t offset, uint16_t selector, uint8_t type)
{
    uint8_t* target = &_IDT[8 * index];

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
        encodeIdtEntry(i, (uint32_t)&ASM_ISR_0, 0x08, TRAP_GATE);
    }

    encodeIdtEntry(0x00, (uint32_t)&_isr0, 0x08, INT_GATE); // Exceptions
    encodeIdtEntry(0x01, (uint32_t)&_isr1, 0x08, INT_GATE);
    encodeIdtEntry(0x02, (uint32_t)&_isr2, 0x08, INT_GATE);
    encodeIdtEntry(0x03, (uint32_t)&_isr3, 0x08, INT_GATE);
    encodeIdtEntry(0x04, (uint32_t)&_isr4, 0x08, INT_GATE);
    encodeIdtEntry(0x05, (uint32_t)&_isr5, 0x08, INT_GATE);
    encodeIdtEntry(0x06, (uint32_t)&_isr6, 0x08, INT_GATE);
    encodeIdtEntry(0x07, (uint32_t)&_isr7, 0x08, INT_GATE);
    encodeIdtEntry(0x08, (uint32_t)&_isr8, 0x08, INT_GATE);
    encodeIdtEntry(0x09, (uint32_t)&_isr9, 0x08, INT_GATE);
    encodeIdtEntry(0x0A, (uint32_t)&_isr10, 0x08, INT_GATE);
    encodeIdtEntry(0x0B, (uint32_t)&_isr11, 0x08, INT_GATE);
    encodeIdtEntry(0x0C, (uint32_t)&_isr12, 0x08, INT_GATE);
    encodeIdtEntry(0x0D, (uint32_t)&_isr13, 0x08, INT_GATE);
    encodeIdtEntry(0x0E, (uint32_t)&_isr14, 0x08, INT_GATE);
    encodeIdtEntry(0x0F, (uint32_t)&_isr15, 0x08, INT_GATE);
    encodeIdtEntry(0x10, (uint32_t)&_isr16, 0x08, INT_GATE);
    encodeIdtEntry(0x11, (uint32_t)&_isr17, 0x08, INT_GATE);
    encodeIdtEntry(0x12, (uint32_t)&_isr18, 0x08, INT_GATE);
    encodeIdtEntry(0x13, (uint32_t)&_isr19, 0x08, INT_GATE);
    encodeIdtEntry(0x14, (uint32_t)&_isr20, 0x08, INT_GATE);
    encodeIdtEntry(0x15, (uint32_t)&_isr21, 0x08, INT_GATE);
    encodeIdtEntry(0x16, (uint32_t)&_isr22, 0x08, INT_GATE);
    encodeIdtEntry(0x17, (uint32_t)&_isr23, 0x08, INT_GATE);
    encodeIdtEntry(0x18, (uint32_t)&_isr24, 0x08, INT_GATE);
    encodeIdtEntry(0x19, (uint32_t)&_isr25, 0x08, INT_GATE);
    encodeIdtEntry(0x1A, (uint32_t)&_isr26, 0x08, INT_GATE);
    encodeIdtEntry(0x1B, (uint32_t)&_isr27, 0x08, INT_GATE);
    encodeIdtEntry(0x1C, (uint32_t)&_isr28, 0x08, INT_GATE);
    encodeIdtEntry(0x1D, (uint32_t)&_isr29, 0x08, INT_GATE);
    encodeIdtEntry(0x1E, (uint32_t)&_isr30, 0x08, INT_GATE);
    encodeIdtEntry(0x1F, (uint32_t)&_isr31, 0x08, INT_GATE);

    encodeIdtEntry(0x20, (uint32_t)&ASM_ISR_PIT, 0x08, INT_GATE);        // PIT
    encodeIdtEntry(0x21, (uint32_t)&ASM_ISR_KBD, 0x08, INT_GATE);        // Keyboard
    encodeIdtEntry(0x69, (uint32_t)&ASM_ISR_SYSCALL, 0x08, TRAP_GATE);   // Syscall

    lidt(&_IDT, sizeof(_IDT));
}