#include <stdint.h>

class GDT_Class {
    public:
        void load();
};

extern "C" 
{
    extern void ASM_RELOAD_SEGMENTS(void);
}


extern GDT_Class GDT;