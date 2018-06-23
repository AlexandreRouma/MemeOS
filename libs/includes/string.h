#include <stdint.h>

uint64_t strlen(const char* str);
bool strcmp(const char* str_a, const char* str_b);
void* memmove(void* dstptr, const void* srcptr, uint64_t size);
void* memcmp(void* aprt, const void* bptr, uint64_t size);
char* dumpHexByte(uint8_t n);
uint32_t strcnt(const char* str, char c);
int strfio(const char* str, char c);
char* substr(char* str, uint32_t index);