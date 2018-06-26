#include <multiboot.h>
#include <stdint.h>
#include <gdt.h>
#include <tss.h>

#define GDT_ENTRY_COUNT 256

GDT_Class GDT;

char _GDT[GDT_ENTRY_COUNT * 8];

void lgdt(void* base, uint16_t size) {   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) GDTR = { size, base };
 
    asm ( "lgdt %0" : : "m"(GDTR) );  // let the compiler choose an addressing mode
}

void encodeGdtEntry(uint32_t index, uint32_t base, uint32_t limit, uint8_t type) {
    char* target = &_GDT[index * 8];

    target[6] = 0xD0;
 
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
    encodeGdtEntry(0x00, 0x00000000, 0x00000000, 0x00); // Null descriptor
    encodeGdtEntry(0x01, 0x00000000, 0xFFFFFFFF, 0x9A); // Code
    encodeGdtEntry(0x02, 0x00000000, 0xFFFFFFFF, 0x92); // Data
    encodeGdtEntry(0x03, 0x00000000, 0x00000000, 0x96); // Stack
    
    // Define task segments
    GDT.setGDTEntry(0x04, 0x01001000, 0x00000000, 0xFF); // ucode
    GDT.setGDTEntry(0x05, 0x01001000, 0x00000000, 0xF3); // udata
    GDT.setGDTEntry(0x06, 0x00000000, 0x00000020, 0xF7); // ustack
    
    initTss();

    encodeGdtEntry(0x07, (uint32_t)&kernel_tss, 0x67, 0xE9);

    lgdt(&_GDT, sizeof(_GDT));
    ASM_RELOAD_SEGMENTS();

    asm("  movw $0x38, %ax \n \
            ltr %ax");

    asm("  movw %%ss, %0 \n \
            movl %%esp, %1" : "=m" (kernel_tss.ss0), "=m" (kernel_tss.esp0) : );
}

void GDT_Class::setGDTEntry(uint32_t index, uint32_t base, uint32_t limit, uint8_t type) {
    encodeGdtEntry(index, base, limit, type);
}