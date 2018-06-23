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
#include <memory.h>
#include <time.h>
#include <paging.h>
#include <std/stdio.h>
#include <syscalls.h>

char* logo = "       ______        \n"
            "  .---<__. \\ \\        \n"
    "  `---._  \\ \\ \\    \n"
    "   ,----`- `.))    \n"
    "  / ,--.   )  |    \n"
    " /_/    >     |    \n"
    " |,\\__-'      |    \n"
    "  \\_           \\    \n"
    "    ~~-___      )    \n"
    "          \\      \\    ";

void shell_main() {
    Terminal.setColor(0x0E);
    Terminal.println(logo);
    Terminal.setColor(0x02);
    Terminal.print("\n\nWelcome to the MemeOS Shell !\n\n");
    Terminal.setColor(0x07);
    Terminal.showCursor(2);

    char* testpointer = malloc(1);

    while (true) {
        Terminal.setColor(0x09);
        Terminal.print("MemeOS");
        Terminal.setColor(0x07);
        Terminal.print("> ");

        // char buffer[1024];                  // Make space for text
        // for (int i = 0; i < 1024; i++) {    // Clear the buffer
        //     buffer[i] = 0x00;
        // }
        char* cmd_str = calloc(1024);// = (char*)&buffer;     // Create a char pointer
        Terminal.readLine(cmd_str);         // Read the keys
        uint16_t cmd_lng = strlen(cmd_str); // Get the length

        Terminal.println("");

        if (cmd_lng == 0) {
            // Nothing
        }
        else if (strcmp("ping", cmd_str)) {
            Terminal.println("Pong !");
        }
        else if (strcmp("clear", cmd_str)) {
            Terminal.clear();
        }
        else if (strcmp("memstats", cmd_str)) {
            uint32_t usedmem = memused();
            uint32_t freemem = memfree();
            uint32_t totalmem = freemem + usedmem;
            Terminal.print(dumpHexByte((usedmem >> 24) & 0xFF));
            Terminal.print(dumpHexByte((usedmem >> 16) & 0xFF));
            Terminal.print(dumpHexByte((usedmem >> 8) & 0xFF));
            Terminal.print(dumpHexByte(usedmem & 0xFF));
            Terminal.print(" used - ");
            Terminal.print(dumpHexByte((freemem >> 24) & 0xFF));
            Terminal.print(dumpHexByte((freemem >> 16) & 0xFF));
            Terminal.print(dumpHexByte((freemem >> 8) & 0xFF));
            Terminal.print(dumpHexByte(freemem & 0xFF));
            Terminal.print(" free - ");
            Terminal.print(dumpHexByte((totalmem >> 24) & 0xFF));
            Terminal.print(dumpHexByte((totalmem >> 16) & 0xFF));
            Terminal.print(dumpHexByte((totalmem >> 8) & 0xFF));
            Terminal.print(dumpHexByte(totalmem & 0xFF));
            Terminal.println(" total");
            free((uint32_t)testpointer);
        }
        else if (strcmp("time", cmd_str)) {
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
        else if (strcmp("reboot", cmd_str)) {
            uint8_t good = 0x02;
            while (good & 0x02) {
                good = inb(0x64);
            }
            outb(0x64, 0xFE);
            asm("hlt");
        }
        else if (strcmp("syscall", cmd_str)) {
            syscall(1, 2, 3, 4);
            printf("gfy", 0x00);
        }
        else if (strcmp("crash", cmd_str)) {
            kernel_panic(0x4269, "Self triggered crash, no real exception here !");
        }
        else {
            Terminal.setColor(0x04);
            Terminal.print("Unknown command: '");
            Terminal.print(cmd_str);
            Terminal.println("'");
            Terminal.setColor(0x07);
            uint32_t ptr = (uint32_t)cmd_str;
        }

        free((uint32_t)cmd_str);
    }
}

void task(void) {
    printf("gfy");
}