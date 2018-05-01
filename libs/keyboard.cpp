#include <stdint.h>
#include "io.h"
#include "keyboard.h"
#include "terminal.h"
#include "string.h"
#include "speaker.h"
#include "pit.h"

#define PRESSED     0
#define RELEASED    1

Keyboard_Class Keyboard;

int last_key_pressed = -1;

void writeToBuffer(uint8_t keycode, uint8_t type) {
    if (type == PRESSED) {
        last_key_pressed = keycode;
        //char * cc = " ";
        //cc[0] = (char)KEYBOARD_LAYOUT_FR_BE[0][keycode];
        //Terminal.print(cc);
        //Terminal.print("PRESSED:  ");
        //Terminal.println(dumpHexByte(keycode));
    }
    else {
        //Terminal.print("RELEASED: ");
        //Terminal.println(dumpHexByte(keycode));
    }
}

uint8_t Keyboard_Class::readKey(bool wait) {
    while (wait && last_key_pressed == -1) {
        asm("nop");
    }
    uint8_t keycode = last_key_pressed;
    last_key_pressed = -1;
    return keycode;
}

void waitIO() {
    uint8_t status = inb(0x64);
    while ((status & 0b00000001) == 1) {
        uint8_t dummy = inb(0x60);
    }
}

void Keyboard_Class::Init() {
    outb(0x64, 0xAD);
    outb(0x64, 0xA7);
    waitIO();
    outb(0x64, 0xAE);
    outb(0x64, 0xA8);
}

bool E0 = false;
uint8_t E1 = 0;

uint8_t last = 0;

void Keyboard_Class::update() {
    uint8_t scancode = inb(0x60);
    if (E1 > 0) {
        E1--;
        return;
    }
    if (scancode == 0xE0) {
        E0 = true;
        return;
    }
    if (scancode == 0xE1) {
        E1 = 3;
        return;
        // Pause Pressed / Released
    }
    if (E0) {
        E0 = false;
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
    else if (E1) {
        E1 = false;
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