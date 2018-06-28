#include <stdint.h>

class ATAPIO_Class {
    public:
        int readBlock(int drive, int numblock, int count, char *buf);
        int writeBlock(int drive, int numblock, int count, char *buf);
};

extern ATAPIO_Class ATAPIO;