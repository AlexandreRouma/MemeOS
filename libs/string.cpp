#include <stdint.h>
#include <stddef.h>

uint64_t strlen(const char* str){
    uint64_t len = 0;
	while (str[len])
		len++;
	return len;
}

bool strcmp(const char* str_a, const char* str_b) {
	if (strlen(str_a) != strlen(str_b)) {
		return false;
	}
	for (uint32_t i = 0; i < strlen(str_a); i++) {
		if (str_a[i] != str_b[i]) {
			return false;
		}
	}
	return true;
}

void* memmove(void* dstptr, const void* srcptr, uint64_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (uint64_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (uint64_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

char HEX_ALPHABET[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char* dumpHexByte(uint8_t n) {
    char* rtn = "00";
    rtn[1] = HEX_ALPHABET[n & 0x0F];
    rtn[0] = HEX_ALPHABET[(n & 0xF0) >> 4];
    return rtn;
}

uint32_t strcnt(const char* str, char c) {
	uint32_t count = 0;
	for (uint32_t i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			count++;
		}
	}
	return count;
}

int strfio(const char* str, char c) {
	uint32_t index = 0;
	for (uint32_t i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			return i;
		}
	}
	return -1;
}

char* substr(char* str, uint32_t index) {
	return str + index;
}