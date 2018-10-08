#include <stdint.h>

enum FloppyRegisters
{
   FD_REG_STATUS_REGISTER_A                = 0x3F0,
   FD_REG_STATUS_REGISTER_B                = 0x3F1,
   FD_REG_DIGITAL_OUTPUT_REGISTER          = 0x3F2,
   FD_REG_TAPE_DRIVE_REGISTER              = 0x3F3,
   FD_REG_MAIN_STATUS_REGISTER             = 0x3F4,
   FD_REG_DATARATE_SELECT_REGISTER         = 0x3F4,
   FD_REG_DATA_FIFO                        = 0x3F5,
   FD_REG_DIGITAL_INPUT_REGISTER           = 0x3F7,
   FD_REG_CONFIGURATION_CONTROL_REGISTER   = 0x3F7
};

enum FloppyCommands
{
   FD_CMD_READ_TRACK =                 2,	// generates IRQ6
   FD_CMD_SPECIFY =                    3,      // * set drive parameters
   FD_CMD_SENSE_DRIVE_STATUS =         4,
   FD_CMD_WRITE_DATA =                 5,      // * write to the disk
   FD_CMD_READ_DATA =                  6,      // * read from the disk
   FD_CMD_RECALIBRATE =                7,      // * seek to cylinder 0
   FD_CMD_SENSE_INTERRUPT =            8,      // * ack IRQ6, get status of last command
   FD_CMD_WRITE_DELETED_DATA =         9,
   FD_CMD_READ_ID =                    10,	// generates IRQ6
   FD_CMD_READ_DELETED_DATA =          12,
   FD_CMD_FORMAT_TRACK =               13,     // *
   FD_CMD_DUMPREG =                    14,
   FD_CMD_SEEK =                       15,     // * seek both heads to cylinder X
   FD_CMD_VERSION =                    16,	// * used during initialization, once
   FD_CMD_SCAN_EQUAL =                 17,
   FD_CMD_PERPENDICULAR_MODE =         18,	// * used during initialization, once, maybe
   FD_CMD_CONFIGURE =                  19,     // * set controller parameters
   FD_CMD_LOCK =                       20,     // * protect controller params from a reset
   FD_CMD_VERIFY =                     22,
   FD_CMD_SCAN_LOW_OR_EQUAL =          25,
   FD_CMD_SCAN_HIGH_OR_EQUAL =         29
};

class FloppyDrive_t {
public:
    uint8_t id;

private:
    
};

class FloppyClass {
public:
    bool init();
    FloppyDrive_t getDrive(uint32_t id);
    void irq();
    void reset();

private:
    bool irq_rcv = false;
    FloppyDrive_t* _drives;
    uint32_t _driveCount;
};

extern FloppyClass Floppy;