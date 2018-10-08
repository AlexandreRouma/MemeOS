#include <stdint.h>
#include <libs/kernel/io.h>
#include <drivers/rtc/time.h>

Time_Class Time;

Time_t Time_Class::getTime() {
    Time_t time;
    outb(0x70, 0x00);
    time.seconds = inb(0x71);
    outb(0x70, 0x02);
    time.minutes = inb(0x71);
    outb(0x70, 0x04);
    time.hours = inb(0x71);
    outb(0x70, 0x07);
    time.day = inb(0x71);
    outb(0x70, 0x08);
    time.month = inb(0x71);
    outb(0x70, 0x09);
    time.year = inb(0x71);
    return time;
}