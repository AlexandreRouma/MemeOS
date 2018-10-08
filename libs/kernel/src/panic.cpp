#include <stdint.h>
#include <libs/kernel/panic.h>
#include <drivers/text_term/terminal.h>
#include <libs/std/string.h>

void kernel_panic(uint16_t error_code, char* message) {
    asm("cli");
    Terminal.hideCursor();
    Terminal.setColor(0x4F);
    Terminal.clear();
    Terminal.setColor(0x70);
    for (int x = 0; x < Terminal.getWidth(); x++) {
        Terminal.putcar(' ', x, 0);
    }
    for (int x = 0; x < Terminal.getWidth(); x++) {
        Terminal.putcar(' ', x, Terminal.getHeight() - 1);
    }
    Terminal.setCursor((Terminal.getWidth() / 2) - 6, 0);
    Terminal.print("KERNEL PANIC");
    Terminal.setCursor(0, Terminal.getHeight() - 1);
    Terminal.print("ERROR CODE: 0x");
    Terminal.print(dumpHexByte((error_code >> 24) & 0xFF));
    Terminal.print(dumpHexByte((error_code >> 16) & 0xFF));
    Terminal.print(dumpHexByte((error_code >> 8) & 0xFF));
    Terminal.print(dumpHexByte(error_code & 0xFF));
    Terminal.setColor(0x4F);
    Terminal.setCursor(2, 3);
    Terminal.println(message);
    Terminal.setCursor(2, 21);
    Terminal.setColor(0x4E);
    Terminal.println("OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code\n  monkeys at our headquarters are working VEWY HAWD to fix this!");

    for (;;) {
        asm("hlt");
    }
}