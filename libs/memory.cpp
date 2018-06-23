#include <stdint.h>
#include <string.h>
#include <serial.h>

#define MAX_MEM 300000

struct UsedArea_t {
    uint32_t addr;
    uint32_t size;
    bool valid;
};

char MEMPAGE[MAX_MEM];

UsedArea_t used[1024];
uint32_t usedCount = 0;

char* malloc(uint32_t size) {
    uint32_t addr = (uint32_t)&MEMPAGE[0];
    for (uint32_t i = 0; i < usedCount; i++) {
        if (addr >= used[i].addr && addr < used[i].addr + used[i].size && used[i].valid == true) {
            addr = used[i].addr + used[i].size;
        }
        else if (addr + size >= used[i].addr && addr + size < used[i].addr + used[i].size && used[i].valid == true) {
            addr = used[i].addr + used[i].size;
        }
    }
    used[usedCount].addr = addr;
    used[usedCount].size = size;
    used[usedCount].valid = true;
    usedCount++;
    return (char*)addr;
}

char* calloc(uint32_t size) {
    char* ptr = malloc(size);
    for (uint32_t i = 0; i < size; i++) {
        ptr[i] = 0x00;
    }
    return ptr;
}

void free(uint32_t addr) {
    for (uint32_t i = 0; i < usedCount; i++) {
        if (used[i].addr == addr) {
            used[i].valid = false;
            if (usedCount > 1) {
                memmove(&used[i + 1], &used[i], sizeof(used[0]) * (usedCount - (i + 1)));
            }
            usedCount--;
            return;
        }
    }
}

uint32_t memused() {
    Serial com1(115200, 0x3F8);
    uint32_t mused = 0;
    for (uint32_t i = 0; i < usedCount; i++) {
        if (used[i].valid) {
            mused += used[i].size;
        }
    }
    return mused;
}

uint32_t memfree() {
    return MAX_MEM - memused();
}