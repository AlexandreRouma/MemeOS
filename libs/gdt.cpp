#include <stddef.h>
#include <stdint.h>
#include "gdt.h"

#define GDT_ENTRY_COUNT 3

GDT_Class GDT;

uint8_t _GDT[GDT_ENTRY_COUNT * 8];

void lgdt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) GDTR = { size, base };
 
    asm ( "lgdt %0" : : "m"(GDTR) );  // let the compiler choose an addressing mode
}

void encodeGdtEntry(uint8_t *target, uint32_t base, uint32_t limit, uint8_t type)
{
    target[6] = 0xC0;
 
    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0x0F;
 
    // Encode the base 
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;
 
    // And... Type
    target[5] = type;
}

void GDT_Class::load() {
    asm("cli");
    encodeGdtEntry(&_GDT[0], 0, 0, 0);  // Null descriptor
    encodeGdtEntry(&_GDT[8], 0, 0xFFFFFFFF, 0x9A);
    encodeGdtEntry(&_GDT[16], 0, 0xFFFFFFFF, 0x92);
    lgdt(&_GDT, sizeof(_GDT));
    ASM_RELOAD_SEGMENTS();
}