#include <stdint.h>

struct Time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint32_t epoch;
};

class Time_Class {
    public:
        Time_t getTime();
};

extern Time_Class Time;