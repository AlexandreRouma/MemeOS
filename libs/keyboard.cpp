#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <terminal.h>
#include <string.h>
#include <speaker.h>
#include <pit.h>
#include <serial.h>
#include <keyboard_layouts.h>

#define PRESSED     1
#define RELEASED    2

Keyboard_Class Keyboard;

KeyboardEvent_t last_event;

void writeToBuffer(uint8_t keycode, uint8_t type) {
    if (type == PRESSED) {
        if (keycode == Keyboard.KEYCODES.NUMLOCK) {
            Keyboard.NumlockState = !Keyboard.NumlockState;
        }
        if (keycode == Keyboard.KEYCODES.CAPSLOCK) {
            Keyboard.CapslockState = !Keyboard.CapslockState;
        }
        if (keycode == Keyboard.KEYCODES.LSHIFT) {
            Keyboard.LSHIFTState = true;
        }
        if (keycode == Keyboard.KEYCODES.RSHIFT) {
            Keyboard.RSHIFTState = true;
        }
        if (keycode == Keyboard.KEYCODES.LCTRL) {
            Keyboard.LCTRLState = true;
        }
        if (keycode == Keyboard.KEYCODES.RCTRL) {
            Keyboard.RCTRLState = true;
        }
        if (keycode == Keyboard.KEYCODES.LALT) {
            Keyboard.LALTState = true;
        }
        if (keycode == Keyboard.KEYCODES.ALTGR) {
            Keyboard.ALTGRState = true;
        }
        KeyboardEvent_t event;
        event.keycode = keycode;
        event.type = PRESSED;
        last_event = event;
    }
    else {
        if (keycode == Keyboard.KEYCODES.LSHIFT) {
            Keyboard.LSHIFTState = false;
        }
        if (keycode == Keyboard.KEYCODES.RSHIFT) {
            Keyboard.RSHIFTState = false;
        }
        if (keycode == Keyboard.KEYCODES.LCTRL) {
            Keyboard.LCTRLState = false;
        }
        if (keycode == Keyboard.KEYCODES.RCTRL) {
            Keyboard.RCTRLState = false;
        }
        if (keycode == Keyboard.KEYCODES.LALT) {
            Keyboard.LALTState = false;
        }
        if (keycode == Keyboard.KEYCODES.ALTGR) {
            Keyboard.ALTGRState = false;
        }
        KeyboardEvent_t event;
        event.keycode = keycode;
        event.type = RELEASED;
        last_event = event;
    }
}

KeyboardEvent_t Keyboard_Class::readEvent(bool wait) {
    while (wait && last_event.type == 0) {
        asm("nop");
    }
    KeyboardEvent_t event = last_event;
    last_event.type = 0;
    return event;
}

char Keyboard_Class::getKeyChar(uint8_t keycode) {
    bool shift = (Keyboard.CapslockState & !Keyboard.ALTGRState) ^ (Keyboard.LSHIFTState | Keyboard.RSHIFTState);
    uint8_t mods = shift | (Keyboard.NumlockState << 1) | (Keyboard.ALTGRState << 2);
    return KEYBOARD_LAYOUT_FR_BE[mods][keycode];
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
    Keyboard.NumlockState = true;
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
            if (scancode <= 0x53) {
                writeToBuffer(scancode + 0x29, PRESSED);
            }
            else if (scancode <= 0x58){
                writeToBuffer(scancode + 0x26, PRESSED);
            }
            else if (scancode <= 0xD3){
                writeToBuffer(scancode - 0x57, RELEASED);
            }
            else {
                writeToBuffer(scancode - 0x5A, RELEASED);
            }
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