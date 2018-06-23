#include <io.h>
#include <pit.h>
#include <string.h>

PIT_Class PIT;

void PIT_Class::Init() {
    int divisor = 1193180 / 1000;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void PIT_Class::delay(uint32_t ms) {
    uint32_t endtick = system_ticks + ms;
    while (system_ticks < endtick) {
        asm("nop");
    }
}