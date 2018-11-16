#include <stdint.h>

inline int floor(float value) {
    return (int)value;
}

inline int ceil(float value) {
    return floor(value) + 1;
}

inline int round(float value) {
    if ((float)(value - (int)value) > 0.5f) {
        return floor(value) + 1;
    }
    return floor(value);
}

