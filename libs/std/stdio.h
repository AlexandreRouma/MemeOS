#include <libs/sys/syscall.h>

inline void printf(char* str) {
    syscall(SYS_PRINT, (uint32_t)str, 0, 0);
}