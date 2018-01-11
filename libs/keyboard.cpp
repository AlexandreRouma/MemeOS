#include <stdint.h>
#include "io.h"
#include "keyboard.h"
#include "terminal.h"
#include "string.h"
#include "keyboard_layouts.h"

#define PRESSED     0
#define RELEASED    1

Keyboard_Class Keyboard;

int last_key_pressed = -1;

void writeToBuffer(uint8_t keycode, uint8_t type) {
    if (type == PRESSED) {
        char * cc = " ";
        cc[0] = (char)KEYBOARD_LAYOUT_FR_BE[0][keycode];
        Terminal.print(cc);
        //Terminal.print("PRESSED:  ");
        //Terminal.println(dumpHexByte(keycode));
    }
    else {
        //Terminal.print("RELEASED: ");
        //Terminal.println(dumpHexByte(keycode));
    }
}

void Keyboard_Class::Init() {
    outb(0x64, 0xAD);
    outb(0x64, 0xA7);
    uint8_t status = inb(0x64);
    while (status & 0b00000001 == 1) {
        uint8_t dummy = inb(0x60);
    }
    outb(0x64, 0xAE);
    outb(0x64, 0xA8);
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