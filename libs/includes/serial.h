#include <stdint.h>

class Serial {
    public:
        Serial(uint16_t baud, uint16_t addr);
        void print(char* str);
        void println(char* str);
        void write(char c);

        uint16_t baudrate;
        uint16_t address;
};