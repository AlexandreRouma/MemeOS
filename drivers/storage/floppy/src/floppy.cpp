#include <drivers/storage/floppy/floppy.h>
#include <libs/kernel/io.h>

const uint8_t IMPLIED_SEEK = 1;
const uint8_t FIFO_DISABLE = 0;
const uint8_t POLL_DISABLE = 1;
const uint8_t THRESHOLD = 8;

FloppyClass Floppy;

bool FloppyClass::init() {
    outb(FD_REG_DATA_FIFO, FD_CMD_VERSION);
    if (inb(FD_REG_DATA_FIFO) != 0x90) {
        return false;
    }

    // Configuration
    outb(FD_REG_DATA_FIFO, FD_CMD_CONFIGURE);
    outb(FD_REG_DATA_FIFO, 0x00);
    uint8_t config = (IMPLIED_SEEK << 6) | (FIFO_DISABLE << 5) | (POLL_DISABLE << 4) | ((THRESHOLD - 1) & 0b00001111);
    outb(FD_REG_DATA_FIFO, config);
    outb(FD_REG_DATA_FIFO, 0x00);
    uint8_t dor = inb(FD_REG_DIGITAL_OUTPUT_REGISTER);
    dor |= 0x08;
    outb(FD_REG_DIGITAL_OUTPUT_REGISTER, dor);

    // Lock
    outb(FD_REG_DATA_FIFO, FD_CMD_LOCK);

    reset();

    // TODO: Add recalibrate for each drive
    return true;
}

FloppyDrive_t FloppyClass::getDrive(uint32_t id) {
    FloppyDrive_t drv;
    return drv;
}

void FloppyClass::irq() {
    irq_rcv = true;
}

void FloppyClass::reset() {
    outb(FD_REG_DATARATE_SELECT_REGISTER, 0x80);
}