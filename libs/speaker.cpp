#include <stdint.h>
#include <speaker.h>
#include <io.h>
#include <pit.h>

Speaker_Class Speaker;

void Speaker_Class::tone(uint16_t frequency) {
    uint32_t Div;
 	uint8_t tmp;

 	Div = 1193180 / frequency; // Find divider
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
}

void Speaker_Class::noTone() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void Speaker_Class::beep() {
    tone(1000);
    PIT.delay(100);
    noTone();
}