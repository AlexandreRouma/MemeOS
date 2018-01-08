#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pit.h"
#include "string.h"

extern "C" // Use C link for kernel_main

void kernel_main(void) 
{
    Terminal.Init(80, 25);
    Terminal.setColor(0x0E);
    Terminal.clear();
    Terminal.hideCursor();
    Terminal.println("Welcome to MemeOS ! (DEBUG 0002)");
    Terminal.setColor(0x07);

    Terminal.print("Loading GDT...   ");
    GDT.load();
    Terminal.OK();

    Terminal.print("Remapping PIC... ");
    PIT.Init();
    Terminal.OK();

    Terminal.print("Loading IDT...   ");
    IDT.load();
    asm("sti");
    Terminal.OK();


    Terminal.println("\n\nMemeOS> ");

    for(;;) {
        asm("hlt");
    }

}