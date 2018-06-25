#include <stdint.h>

class GDT_Class {
    public:
        void load();
        void setGDTEntry(uint32_t index, uint32_t base, uint32_t limit, uint8_t type);
};

extern "C" 
{
    extern void ASM_RELOAD_SEGMENTS(void);
}


extern GDT_Class GDT;