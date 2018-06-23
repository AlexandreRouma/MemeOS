#include <stdint.h>
#include <panic.h>
#include <terminal.h>
#include <string.h>

void kernel_panic(uint16_t error_code, char* message) {
    asm("cli");
    Terminal.hideCursor();
    Terminal.setColor(0x4F);
    Terminal.clear();
    Terminal.setColor(0xF4);
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
    Terminal.print(dumpHexByte((error_code >> 8) & 0xFF));
    Terminal.print(dumpHexByte(error_code & 0xFF));
    Terminal.setColor(0x4F);
    Terminal.setCursor(2, 3);
    Terminal.println(message);

    for (;;) {
        asm("hlt");
    }
}