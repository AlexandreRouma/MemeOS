#include <stdint.h>
#include <drivers/text_term/terminal.h>
#include <libs/kernel/io.h>
#include <libs/kernel/gdt.h>
#include <libs/kernel/idt.h>
#include <libs/kernel/pic.h>
#include <drivers/timer/pit.h>
#include <drivers/pc_speaker/speaker.h>
#include <libs/std/string.h>
#include <libs/kernel/panic.h>
#include <drivers/keyboard/keyboard.h>
#include <libs/kernel/multiboot.h>
#include <libs/kernel/paging.h>
#include <drivers/pci/pci.h>
#include <drivers/storage/ahci/ahci.h>
#include <drivers/serial/serial.h>
#include <drivers/storage/floppy/floppy.h>
#include <drivers/storage/atapio/atapio.h>
#include "shell/shell.h"
#include <drivers/storage/drive.h>
#include <libs/kernel/tss.h>
#include <libs/std/stream.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

struct VBE2Block_t {
    uint8_t signature[4];
    uint8_t ver_maj;
    uint8_t ver_min;
};

void test(iostream<char> strm) {
    return;
    char* data = "Hello from stream\n";
    strm.write(data, 18);
}

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

    Serial serial(115200, 0x3F8);

    test(serial.outStream);
    //test(Terminal.outStream);

    Terminal.println("Welcome to MemeOS v1.8 !");
    Terminal.setColor(0x07);
    
    Terminal.print("Loading GDT...                          ");
    GDT.load();
    Terminal.OK();

    Terminal.print("Enabling paging...                      ");
    Paging.enable(multiboot_info->mem_upper / 4);
    Terminal.OK();

    // Memory Management can now be used!

    Terminal.print("Remapping PIC...                        ");
    PIC.Init();
    Terminal.OK();

    Terminal.print("Loading IDT...                          ");
    IDT.load();
    Terminal.OK();

    // Interrupts can now be used!

    Terminal.print("Loading TSS...                          ");
    initTss();
    loadTss();
    Terminal.OK();

    Terminal.print("Configuring PIT...                      ");
    PIT.Init();
    Terminal.OK();

    Terminal.print("Initialising Keyboard...                ");
    Keyboard.Init();
    Terminal.OK();

    asm("sti");

    Terminal.print("Finding ACPI pointer...                 ");

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

    if (RSDPTR != 0) {
        Terminal.OK();
    }
    else {
        Terminal.FAILED();
    }


    Terminal.print("Scanning PCI devices...                 ");
    PCI.scanDevices();
    Terminal.OK();

    // Terminal.print("Initializing floppy controller...       ");
    // if (Floppy.init()) {
    //     Terminal.OK();
    // }
    // else {
    //     Terminal.FAILED();
    // }
    
    Terminal.print("Scanning ATA devices...                 ");
    // Enable ata shit here
    Terminal.OK();

    // -drive id=disk,file=hda.img,if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0
    //Terminal.print("Scanning ACHI controllers... ");
    //AHCI.scanControllers();
    //Terminal.OK();

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