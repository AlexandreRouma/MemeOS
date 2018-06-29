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
#include <terminal.h>
#include <serial.h>
#include <liballoc.h>
#include <time.h>
#include <paging.h>
#include <std/stdio.h>
#include <syscalls.h>
#include <atapio.h>
#include <ext2fs.h>
#include <pci.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

void task1(void) {
    printf("Beep beep letuce from userspace task");
    while(1);
    return;
}

void shell_main(MultibootInfo_t* boot_info) {
    Terminal.setColor(0x02);
    Terminal.print("\nDankBASH v1.1, logged in as root, ");
    Terminal.print(itoa(boot_info->mem_upper / 1024, 10));
    Terminal.println("Mb free.\n");

    Terminal.setColor(0x07);
    Terminal.showCursor(2);

    while (true) {
        Terminal.setColor(0x09);
        Terminal.print("MemeOS");
        Terminal.setColor(0x07);
        Terminal.print("> ");

        string cmd_str = Terminal.readLine();

        Terminal.println("");

        if (cmd_str == "") {
            // Nothing
        }
        else if (cmd_str.startsWith("echo")) {
            if (cmd_str.length() > 5) {
                Terminal.println(cmd_str.substring(5));
            }
            else {
                Terminal.setColor(0x04);
                Terminal.println("No argument !");
                Terminal.setColor(0x07);
            }
        }
        else if (cmd_str == "memstats") {
            uint32_t used = Paging.getUsedPages() / 256;
            Terminal.print(itoa(used, 10));
            Terminal.print("Mb used, ");
            Terminal.print(itoa((boot_info->mem_upper / 1024) - used, 10));
            Terminal.print("Mb free, ");
            Terminal.print(itoa(boot_info->mem_upper / 1024, 10));
            Terminal.print("Mb total (");
            float percent = ((float)used / (float)(boot_info->mem_upper / 1024)) * 100;
            Terminal.print(itoa(percent, 10));
            Terminal.println("%)");
        }
        else if (cmd_str == "ping") {
            Terminal.println("Pong !");
        }
        else if (cmd_str == "clear") {
            Terminal.clear();
        }
        else if (cmd_str == "hddtest") {
            parseFs();
        }
        else if (cmd_str == "lspci") {
            for (uint8_t id = 0; id < PCI.getDeviceCount(); id++) {
                PCIDevice_t dev = PCI.getDevice(id);

                Terminal.print(itoa(dev.bus, 16));
                Terminal.print(":");
                Terminal.print(itoa(dev.slot, 16));
                Terminal.print(".");
                Terminal.print(itoa(dev.function, 16));
                Terminal.print(" - ");

                Terminal.print(dev.deviceChip);
                Terminal.print(": ");

                Terminal.print(dev.vendorFullName);
                Terminal.print(", ");
                Terminal.print(dev.deviceChipDesc);
                Terminal.print(" (rev ");
                Terminal.print(itoa(dev.revisionID, 10));
                Terminal.println(")");
            }
        }
        else if (cmd_str == "rapemem") {
            for (int i = 0; i < 10; i++) {
                void* ptr = malloc(1024 * 100);
            }
            Terminal.println("DED xD");
        }
        else if (cmd_str == "teststr") {
            string str = "Hello World!";
            if (str.endWith("World!")) {
                Terminal.println(str.toLower());
            }
        }
        else if (cmd_str == "time") {
            Time_t time = Time.getTime();
            Terminal.print(dumpHexByte(time.hours));
            Terminal.print(":");
            Terminal.print(dumpHexByte(time.minutes));
            Terminal.print(":");
            Terminal.println(dumpHexByte(time.seconds));
            Terminal.print(dumpHexByte(time.day));
            Terminal.print("/");
            Terminal.print(dumpHexByte(time.month));
            Terminal.print("/");
            Terminal.println(dumpHexByte(time.year));
        }
        else if (cmd_str == "reboot") {
            uint8_t good = 0x02;
            while (good & 0x02) {
                good = inb(0x64);
            }
            outb(0x64, 0xFE);
            asm("hlt");
        }
        else if (cmd_str == "syscall") {
            syscall(1, 2, 3, 4);
        }
        else if (cmd_str == "crash") {
            kernel_panic(0x4269, "Self triggered crash, no real exception here !");
        }
        else if (cmd_str == "task") {
            // Task map:
            // 0xF0000000 - 0xF0001000 => ustack
            // 0xF0001000 - 0xF0002000 => ucode, udata

            // Allocate pages
            Paging.setPresent(0x01000000, 1); // ustack
            Paging.setPresent(0x01001000, 1); // ucode

            Paging.setDirectoryFlags(0x01000000, 0x07);
            Paging.setDirectoryFlags(0x01001000, 0x07);
            Paging.setFlags(0x01000000, 0x07);
            Paging.setFlags(0x01001000, 0x07);

            // Define task segments
            GDT.setGDTEntry(0x05, 0x01001000, 0x00000000, 0xFF); // ucode
            GDT.setGDTEntry(0x06, 0x01001000, 0x00000000, 0xF3); // udata
            GDT.setGDTEntry(0x07, 0x00000000, 0x00000020, 0xF7); // ustack

            // Load task
            memmove((void*)0x01001000, (void*)&task1, 0xFFF);

            uint32_t st = (uint32_t)&ASM_STACK_TOP;

            asm("   cli \n \
                    push $0x3B \n \
                    push $0x01001000 \n \
                    pushfl \n \
                    popl %%eax \n \
                    orl $0x200, %%eax \n \
                    and $0xffffbfff, %%eax \n \
                    push %%eax \n \
                    push $0x2B \n \
                    push $0x0 \n \
                    movl $ASM_STACK_TOP, %0 \n \
                    movw $0x33, %%ax \n \
                    movw %%ax, %%ds \n \
                    iret" : "=m" (st) : );
        }
        else {
            Terminal.setColor(0x04);
            Terminal.print("Unknown command: '");
            Terminal.print(cmd_str);
            Terminal.println("'");
            Terminal.setColor(0x07);
        }
    }
}