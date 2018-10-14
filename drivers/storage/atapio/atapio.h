#include <stdint.h>
#include <drivers/storage/drive.h>

class ATAPIODrive_t : public Drive_t {
public:
    ATAPIODrive_t(uint8_t id);
    bool read(uint64_t addr, uint64_t size, uint8_t* buf);
    bool write(uint64_t addr, uint64_t size, uint8_t* buf);
    //int getCapacity(float) = 0 ; // TODO: Finish

private:
    uint8_t id;
};

class ATAPIO_Class {
public:
    int readBlock(int drive, int numblock, int count, char *buf);
    int writeBlock(int drive, int numblock, int count, char *buf);
};

extern ATAPIO_Class ATAPIO;