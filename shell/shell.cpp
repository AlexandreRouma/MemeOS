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
#include <drivers/text_term/terminal.h>
#include <drivers/serial/serial.h>
#include <libs/kernel/liballoc.h>
#include <drivers/rtc/time.h>
#include <libs/kernel/paging.h>
#include <libs/std/stdio.h>
#include <libs/kernel/syscalls.h>
#include <drivers/storage/atapio/atapio.h>
#include <drivers/filesystems/ext2/ext2fs.h>
#include <drivers/pci/pci.h>
#include <drivers/storage/ahci/ahci.h>
#include <libs/std/vector.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

extern "C" {
    extern void* _END_ASM_USER_TASK;
    extern void* ASM_USER_TASK;
}

void task1(void) {
    printf("Beep beep letuce from userspace task");
    while(1);
    return;
}

uint32_t testmap = 0;

void shell_main(MultibootInfo_t* boot_info) {
    Terminal.setColor(0x02);
    Terminal.print("\nDankBASH v1.1, logged in as root, ");
    Terminal.print(itoa(boot_info->mem_upper / 1024, 10));
    Terminal.println("Mb free.\n");

    Terminal.setColor(0x07);
    Terminal.showCursor(2);

    while (true) {
        vector<int> ints;
        ints.push_back(420);
        ints.pop_back();
        ints.push_front(420);
        ints.pop_front();
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
            //parseFs();
            ATAPIODrive_t drive(0);
            char* strlol = "Good evening peasants!";
            drive.write(0x7FA, 22, (uint8_t*)strlol);
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
        else if (cmd_str == "lsblk") {
            // for (uint8_t id = 0; id < AHCI.getControllerCount(); id++) {
            //     ACHIController_t cnt = AHCI.getController(id);
            //     for (uint8_t d = 0; d < cnt.driveCount; d++) {
            //         ACHIDrive_t drv = cnt.drives[d];
            //         Terminal.print(itoa(drv.controllerID, 10));
            //         Terminal.print(".");
            //         Terminal.print(itoa(d, 10));
            //         Terminal.print(": ");
            //         if (drv.type == AHCI_DEV_SATA) {
            //             Terminal.println("SATA");
            //         }
            //         if (drv.type == AHCI_DEV_SEMB) {
            //             Terminal.println("SEMB");
            //         }
            //         if (drv.type == AHCI_DEV_PM) {
            //             Terminal.println("PM");
            //         }
            //         if (drv.type == AHCI_DEV_SATAPI) {
            //             Terminal.println("SATAPI");
            //         }
            //     }
            // }
            Terminal.println("WARNING: Unimplemented");
        }
        else if (cmd_str == "rapemem") {
            for (int i = 0; i < 100; i++) {
                void* ptr = malloc(1024);
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
        else if (cmd_str == "pagemap") {
            uint32_t phy = Paging.allocPages(1);
            Paging.mapPage(0x000F0000, phy, 0b11);
            Terminal.print("0x00000000 is now mapped to 0x");
            Terminal.println(itoa(phy, 16));
            testmap = phy;
        }
        else if (cmd_str == "checkmap") {
            uint8_t* ptr1 = (uint8_t*)0x000F0000;
            uint8_t* ptr2 = (uint8_t*)testmap;
            bool failed = false;
            for (int i = 0; i < 4096; i++) {
                if (ptr1[i] != ptr2[i]) {
                    Terminal.print("Missmatch at 0x");
                    Terminal.println(itoa(i, 16));
                    failed = true;
                }
            }
            if (!failed) {
                Terminal.println("Mapping worked!");
            }
        }
        else if (cmd_str == "tasktest") {
            Terminal.println(itoa((uint32_t)ASM_USER_TASK, 16));
            Terminal.println(itoa((uint32_t)_END_ASM_USER_TASK, 16));
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
            // GDT.setGDTEntry(0x05, 0x01001000, 0x00000000, 0xFF); // ucode
            // GDT.setGDTEntry(0x06, 0x01001000, 0x00000000, 0xF3); // udata
            // GDT.setGDTEntry(0x07, 0x00000000, 0x00000020, 0xF7); // ustack

            GDT.allocEntry(0x01001000, 0x00000000, GDT_ACCESS_PRESENT | GDT_ACCESS_EXEC | GDT_ACCESS_RW | GDT_ACCESS_PRIV_3, GDT_FLAG_GR_PAGE | GDT_FLAG_SZ_32B);
            GDT.allocEntry(0x01001000, 0x00000000, GDT_ACCESS_PRESENT | GDT_ACCESS_EXEC | GDT_ACCESS_RW | GDT_ACCESS_PRIV_3, GDT_FLAG_GR_PAGE | GDT_FLAG_SZ_32B);
            GDT.allocEntry(0x00000000, 0x00000020, GDT_ACCESS_PRESENT | GDT_ACCESS_DIR_D | GDT_ACCESS_RW | GDT_ACCESS_PRIV_3, GDT_FLAG_SZ_32B);

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