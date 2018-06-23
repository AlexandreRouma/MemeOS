#include <stdint.h>

class Paging_Class {
    public:
        void enable();
        void mapPage(uint32_t phy, uint32_t virt, uint16_t flags);
        uint32_t getPhysicalAddr(uint32_t virt);
        uint16_t getFlags(uint32_t virt);
        void setFlags(uint32_t virt, uint16_t flags);
};

extern "C" 
{
    extern void ASM_LOAD_PAGE_DIRECTORY(uint32_t*);
    extern void ASM_ENABLE_PAGING(void);
}


extern Paging_Class Paging;