#include <stdint.h>
#include <terminal.h>
#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>
#include <speaker.h>
#include <string.h>
#include <panic.h>
#include <keyboard.h>
#include <multiboot.h>
#include <paging.h>
#include <pci.h>
#include <ahci.h>
#include "shell/shell.h"

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

extern "C" // Use C link for kernel_main

void kernel_main(uint32_t multiboot_magic, MultibootInfo_t* multiboot_info) 
{
    Terminal.Init(80, 25);
    Terminal.hideCursor();
    Terminal.setColor(0x0E);
    Terminal.clear();

    if (multiboot_magic != 0x2BADB002) {
        kernel_panic(0xFFFF, "Invalid multiboot signature !");
    }

    Terminal.println("Welcome to MemeOS v1.5 !");
    Terminal.setColor(0x07);

    Terminal.print("Loading GDT...               ");
    GDT.load();
    Terminal.OK();

    Terminal.print("Remapping PIC...             ");
    PIC.Init();
    Terminal.OK();

    Terminal.print("Loading IDT...               ");
    IDT.load();
    Terminal.OK();

    Terminal.print("Configuring PIT...           ");
    PIT.Init();
    Terminal.OK();

    Terminal.print("Initialising Keyboard...     ");
    Keyboard.Init();
    Terminal.OK();

    Terminal.print("Enabling paging...           ");
    Paging.enable();
    Terminal.OK();

    asm("sti");

    Terminal.print("Finding ACPI pointer...      ");

    uint32_t RSDPTR = 0x00000000;
    char* RSDSIG = "RSD PTR ";
    char* RAMPTR = 0x00000000;

    for (uint32_t i = 0; i < 0x100000; i++) {
        uint8_t match = 0;
        for (uint8_t j = 0; j < 8; j++) {
            if (RAMPTR[i + j] == RSDSIG[j]) {
                match++;
            }
        }
        if (match >= 8) {
            RSDPTR = i;
            break;
        }
    }

    Terminal.OK();

    Terminal.print("Scanning PCI devices...      ");
    PCI.scanDevices();
    Terminal.OK();

    Terminal.print("Scanning ACHI controllers... ");
    AHCI.scanControllers();
    Terminal.OK();

    Terminal.setColor(0x03);

    Terminal.println("");

    Terminal.print("Kernel end:   0x");
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 24));
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 16));
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 8));
    Terminal.println(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 0));

    Terminal.print("Stack top:    0x");
    Terminal.print(dumpHexByte((uint32_t)ASM_STACK_TOP >> 24));
    Terminal.print(dumpHexByte((uint32_t)ASM_STACK_TOP >> 16));
    Terminal.print(dumpHexByte((uint32_t)ASM_STACK_TOP >> 8));
    Terminal.println(dumpHexByte((uint32_t)ASM_STACK_TOP >> 0));

    Terminal.print("RSDPTR:       0x");
    Terminal.print(dumpHexByte(RSDPTR >> 24));
    Terminal.print(dumpHexByte(RSDPTR >> 16));
    Terminal.print(dumpHexByte(RSDPTR >> 8));
    Terminal.println(dumpHexByte(RSDPTR >> 0));

    Terminal.setColor(0x07);
    
    shell_main(multiboot_info);

    for (;;) {
        asm("hlt");
    }
}