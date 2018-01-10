#include <stdint.h>
#include "io.h"
#include "keyboard.h"
#include "terminal.h"
#include "string.h"

#define PRESSED     0
#define RELEASED    1

Keyboard_Class Keyboard;

void writeToBuffer(uint8_t keycode, uint8_t type) {
    if (type == PRESSED) {
        Terminal.print("PRESSED:  ");
        Terminal.println(dumpHexByte(keycode));
    }
    else {
        Terminal.print("RELEASED: ");
        Terminal.println(dumpHexByte(keycode));
    }
}

void Keyboard_Class::update() {
    uint8_t scancode = inb(0x60);
    if (scancode == 0xE0) {
        scancode = inb(0x60);
        if (scancode == 0x2A) {
            scancode = inb(0x60);
            scancode = inb(0x60);
            // PrtScr Pressed
        }
        else if (scancode == 0xB7) {
            scancode = inb(0x60);
            scancode = inb(0x60);
            // PrtScr Released
        }
        else {
            
        }
    }
    else if (scancode == 0xE1) {
        scancode = inb(0x60);
        scancode = inb(0x60);
        scancode = inb(0x60);
        scancode = inb(0x60);
        scancode = inb(0x60);
        // Pause Pressed / Released
    }
    else {
        if (scancode <= 0x53) {
            writeToBuffer(scancode - 0x01, PRESSED);
        }
        else if (scancode <= 0x58){
            writeToBuffer(scancode - 0x04, PRESSED);
        }
        else if (scancode <= 0xD3){
            writeToBuffer(scancode - 0x81, RELEASED);
        }
        else {
            writeToBuffer(scancode - 0x84, RELEASED);
        }
    }
}