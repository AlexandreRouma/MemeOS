#include <stdint.h>

class Speaker_Class {
    public:
        void tone(uint16_t frequency);
        void noTone();
        void beep();
};

extern Speaker_Class Speaker;