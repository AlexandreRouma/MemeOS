#include <string.h>
#include <stdint.h>
#include <terminal.h>

uint32_t strlen(char* str){
    uint32_t len = 0;
	while (str[len])
		len++;
	return len;
}

bool strcmp(char* str_a, char* str_b) {
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

void* memmove(void* dstptr, void* srcptr, uint64_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	unsigned char* src = (unsigned char*) srcptr;
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

string itoa(uint32_t n, uint8_t base) {
	string ret = "";
	while (n > 0) {
		uint32_t digit = n % base;
		string str = "";
		str += HEX_ALPHABET[digit];
		ret = str + ret;
		n -= digit;
		n /= base;
	}
	return ret;
}

uint32_t strcnt(char* str, char c) {
	uint32_t count = 0;
	for (uint32_t i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			count++;
		}
	}
	return count;
}

int strfio(char* str, char c) {
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

string::string(char* str) {
    this->_str = (char*)malloc(strlen(str) + 1);
	for (uint32_t i = 0; i < strlen(str) + 1; i++) {
		this->_str[i] = str[i];
	}
}

string::string() {
    this->_str = (char*)malloc(1);
	this->_str[0] = 0x00;
}

string::~string() {
	free(this->_str);
}

char* string::toCharArray() {
	return this->_str;
}

uint32_t string::length() {
	return strlen(this->_str);
}

void string::reserve(uint32_t len) {
	uint32_t old_len = this->length();
	this->_str = (char*)realloc(this->_str, len + 1);
	if (len < old_len) {
		this->_str[len] = 0x00;
	}
	else {
		for (uint32_t i = old_len; i < len + 1; i++) {
			this->_str[i] = 0x00;
		}
	}
}

string string::operator+(string str) {
	string ret;
	ret.reserve(this->length() + str.length());
	memmove(ret.toCharArray(), this->toCharArray(), this->length());
	memmove(ret.toCharArray() + this->length(), str.toCharArray(), str.length() + 1);
	return ret;
}

string string::operator+(char c) {
	string ret;
	ret.reserve(this->length() + 1);
	memmove(ret.toCharArray(), this->toCharArray(), this->length());
	ret.toCharArray()[this->length()] = c;
	return ret;
}

void string::operator+=(string str) {
	this->reserve(this->length() + str.length());
	memmove(this->toCharArray() + this->length(), str.toCharArray(), str.length() + 1);
}

void string::operator+=(char c) {
	this->reserve(this->length() + 1);
	this->toCharArray()[this->length()] = c;
}

bool string::operator==(string str) {
	return strcmp(this->_str, str.toCharArray());
}

bool string::operator!=(string str) {
	return !strcmp(this->_str, str.toCharArray());
}

bool string::operator==(char* str) {
	return strcmp(this->_str, str);
}

bool string::operator!=(char* str) {
	return !strcmp(this->_str, str);
}

char string::operator[](uint32_t i) {
	return this->_str[i];
}

string string::substring(uint32_t index) {
	string ret;
	ret.reserve(this->length() - index);
	memmove(ret.toCharArray(), this->toCharArray() + index, this->length() - index);
	return ret;
}

string string::substring(uint32_t index, uint32_t length) {
	string ret;
	ret.reserve(this->length() - index - (this->length() - index - length));
	memmove(ret.toCharArray(), this->toCharArray() + index, this->length() - index - (this->length() - index - length));
	return ret;
}

bool string::startsWith(string str) {
	for (uint32_t i = 0; i < str.length(); i++) {
		if (str[i] != this->_str[i]) {
			return false;
		}
	}
	return true;
}

bool string::endWith(string str) {
	for (int i = str.length() - 1; i > -1; i--) {
		if (str[i] != this->_str[this->length() - str.length() + i]) {
			return false;
		}
	}
	return true;
}

string string::toUpper() {
	string ret;
	ret.reserve(this->length());
	char* ptr = ret.toCharArray();
	for (uint32_t i = 0; i < this->length(); i++) {
		char c = this->_str[i];
		if (c >= 97 && c <= 122) {
			c &= 0b11011111;
		}
		ptr[i] = c;
	}
	return ret;
}

string string::toLower() {
	string ret;
	ret.reserve(this->length());
	char* ptr = ret.toCharArray();
	for (uint32_t i = 0; i < this->length(); i++) {
		char c = this->_str[i];
		if (c >= 65 && c <= 90) {
			c |= 0b00100000;
		}
		ptr[i] = c;
	}
	return ret;
}