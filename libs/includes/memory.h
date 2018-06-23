#include <stdint.h>

char* malloc(uint32_t size);
char* calloc(uint32_t size);
void free(uint32_t addr);
uint32_t memused();
uint32_t memfree();