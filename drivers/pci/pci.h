#pragma once
#include <stdint.h>
#include <libs/std/vector.h>

#define PCI_CONFIG_ADDRESS  0x0CF8
#define PCI_CONFIG_DATA     0x0CFC

struct PCIStandardHeader_t {
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t cardbusCISPtr;
    uint16_t subsysVendorID;
    uint16_t subsysID;
    uint32_t expROMAddr;
    uint8_t capPtr;
    uint8_t intLine;
    uint8_t intPIN;
    uint8_t minGrant;
    uint8_t maxLantency;
};

struct PCIP2PHeader_t {
    uint32_t BAR0;
    uint32_t BAR1;
    uint8_t primBusNumber;
    uint8_t secBusNumber;
    uint8_t subBusNumber;
    uint8_t secLatencyTimer;
    uint8_t ioBase;
    uint8_t ioLimit;
    uint16_t secStatus;
    uint16_t memBase;
    uint16_t memLimit;
    uint16_t prefetchMemBase;
    uint16_t prefetchMemLimit;
    uint32_t prefetchBaseUpper;
    uint32_t prefetchLimitUpper;
    uint16_t ioBaseUpper;
    uint16_t ioLimitUpper;
    uint8_t capPtr;
    uint8_t intLine;
    uint8_t intPIN;
};

struct PCIP2CHeader_t {
    uint32_t cardbusBaseAddr;
    uint8_t capListOffset;
    uint16_t secStatus;
    uint8_t pciBusNumber;
    uint8_t cardbusBusNuber;
    uint8_t subBusNumber;
    uint8_t cardbusLatTimer;
    uint32_t memBaseAddr0;
    uint32_t memLimit0;
    uint32_t memBaseAddr1;
    uint32_t memLimit1;
    uint32_t ioBaseAddr0;
    uint32_t ioLimit0;
    uint32_t ioBaseAddr1;
    uint32_t ioLimit1;
    uint8_t intLine;
    uint8_t intPIN;
    uint16_t subsysDeviceID;
    uint16_t subsysVendorID;
    uint32_t legacyBaseAddr;
};

struct PCIDevice_t {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint16_t vendorID;
    uint16_t deviceID;
    uint8_t revisionID;
    uint8_t progIF;
    uint8_t subclass;
    uint8_t classCode;
    uint8_t cacheLineSize;
    uint8_t latencyTimer;
    uint8_t headerType;
    uint8_t bist;
    char* vendorShortName;
    char* vendorFullName;
    char* deviceChip;
    char* deviceChipDesc;
    PCIStandardHeader_t standardHeader; // headerType = 0x00
    PCIP2PHeader_t pciToPciHeader;      // headerType = 0x01
    PCIP2CHeader_t pciToCardbusHeader;  // headerType = 0x02
};

class PCI_Class {
    public:
        uint16_t getVendorID(uint8_t bus, uint8_t slot, uint8_t function);
        char* getVendorShortName(uint16_t vendorID);
        char* getVendorFullName(uint16_t vendorID);
        uint16_t getDeviceID(uint8_t bus, uint8_t slot, uint8_t function);
        char* getDeviceName(uint16_t vendorID, uint16_t deviceID);
        char* getDeviceDescription(uint16_t vendorID, uint16_t deviceID);
        PCIDevice_t getDevice(uint8_t bus, uint8_t slot, uint8_t function);
        PCIDevice_t getDevice(uint32_t id);
        void scanDevices();
        uint32_t getDeviceCount();

    private:
        vector<PCIDevice_t> _devices;
};

extern PCI_Class PCI;