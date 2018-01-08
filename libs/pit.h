#include <stdint.h>

inline uint32_t system_ticks = 0;

class PIT_Class {
    public:
        void Init();
        void delay(uint32_t ms);
};

extern PIT_Class PIT;