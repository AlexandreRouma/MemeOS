#include <stddef.h>
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
#include "shell/shell.h"

extern "C" // Use C link for kernel_main

void kernel_main(uint32_t multiboot_magic, MultibootInfo_t* multiboot_info) 
{
    Terminal.Init(80, 25);
    Terminal.hideCursor();
    Terminal.setColor(0x0E);
    Terminal.clear();

    if (multiboot_magic != 0x2BADB002) {
        kernel_panic(0x0000, "Invalid multiboot signature !");
    }

    Terminal.print("Welcome to MemeOS ! Kernel End: 0x");
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 24));
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 16));
    Terminal.print(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 8));
    Terminal.println(dumpHexByte(((uint32_t)(&ASM_KERNEL_END)) >> 0));
    Terminal.setColor(0x07);

    Terminal.print("Loading GDT...           ");
    GDT.load();
    Terminal.OK();

    Terminal.print("Remapping PIC...         ");
    PIC.Init();
    Terminal.OK();

    Terminal.print("Loading IDT...           ");
    IDT.load();
    Terminal.OK();

    Terminal.print("Configuring PIT...       ");
    PIT.Init();
    Terminal.OK();

    Terminal.print("Enabling paging...       ");
    Paging.enable();
    Terminal.OK();

    Terminal.print("Initialising Keyboard... ");
    Keyboard.Init();
    Terminal.OK();

    asm("sti");

    shell_main();

    for (;;) {
        asm("hlt");
    }
}