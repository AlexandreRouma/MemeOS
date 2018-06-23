#include <stdint.h>

class PIT_Class {
    public:
        void Init();
        void delay(uint32_t ms);
        uint64_t system_ticks = 0;
};

extern PIT_Class PIT;