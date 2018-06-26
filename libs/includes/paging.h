#include <stdint.h>

class Paging_Class {
    public:
        void enable();
        void mapPage(uint32_t phy, uint32_t virt, uint16_t flags);
        uint32_t getPhysicalAddr(uint32_t virt);
        uint16_t getFlags(uint32_t virt);
        void setFlags(uint32_t virt, uint16_t flags);
        uint16_t getDirectoryFlags(uint32_t virt);
        void setDirectoryFlags(uint32_t virt, uint16_t flags);
        void setPresent(uint32_t virt, uint32_t count);
        void setAbsent(uint32_t virt, uint32_t count);
        uint32_t findPages(uint32_t count);
        uint32_t allocPages(uint32_t count);
};

extern "C" 
{
    extern void ASM_LOAD_PAGE_DIRECTORY(uint32_t*);
    extern void ASM_ENABLE_PAGING(void);
}


extern Paging_Class Paging;