#include <stdint.h>

#define SYS_PRINT   0x00

inline void syscall(uint32_t id, uint32_t ebx, uint32_t ecx, uint32_t edx) {
    asm("int $0x69" :: "a" (id), "b" (ebx), "c" (ecx), "d" (edx));
}