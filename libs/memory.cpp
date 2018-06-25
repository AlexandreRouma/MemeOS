#include <stdint.h>
#include <string.h>
#include <serial.h>



char* malloc(uint32_t size) {
    
}

char* calloc(uint32_t size) {
    // Malloc but clears the area
}

void free(uint32_t addr) {
    
}

uint32_t memused() {
    
}

uint32_t memfree() {
    return MAX_MEM - memused();
}