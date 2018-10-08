#include <stdint.h>
#include <drivers/pci/pci.h>
#include <libs/kernel/io.h>
#include <libs/std/string.h>
#include <drivers/text_term/terminal.h>
#include <drivers/pci/pcivendors.h>

PCI_Class PCI;

uint16_t readConfig(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t data = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    outl(PCI_CONFIG_ADDRESS, address);
    data = (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return data;
}

uint16_t PCI_Class::getVendorID(uint8_t bus, uint8_t slot, uint8_t function){
    return readConfig(bus, slot, function, 0);
}

char* PCI_Class::getVendorShortName(uint16_t vendorID){
    for (uint16_t i = 0; i < PCI_VENTABLE_LEN; i++) {
        if (PciVenTable[i].VenId == vendorID) {
            return PciVenTable[i].VenShort;
        }
    }
    return "UNKNOWN";
}

char* PCI_Class::getVendorFullName(uint16_t vendorID){
    for (uint16_t i = 0; i < PCI_VENTABLE_LEN; i++) {
        if (PciVenTable[i].VenId == vendorID) {
            return PciVenTable[i].VenFull;
        }
    }
    return "UNKNOWN";
}

uint16_t PCI_Class::getDeviceID(uint8_t bus, uint8_t slot, uint8_t function){
    return readConfig(bus, slot, function, 2);
}

char* PCI_Class::getDeviceName(uint16_t vendorID, uint16_t deviceID){
    for (uint16_t i = 0; i < PCI_DEVTABLE_LEN; i++) {
        if (PciDevTable[i].VenId == vendorID && PciDevTable[i].DevId == deviceID) {
            return PciDevTable[i].Chip;
        }
    }
    return "UNKNOWN";
}

char* PCI_Class::getDeviceDescription(uint16_t vendorID, uint16_t deviceID){
    for (uint16_t i = 0; i < PCI_DEVTABLE_LEN; i++) {
        if (PciDevTable[i].VenId == vendorID && PciDevTable[i].DevId == deviceID) {
            return PciDevTable[i].ChipDesc;
        }
    }
    return "UNKNOWN";
}

PCIDevice_t PCI_Class::getDevice(uint8_t bus, uint8_t slot, uint8_t function) {
    PCIDevice_t dev;
    dev.bus = bus;
    dev.slot = slot;
    dev.function = function;
    dev.vendorID = readConfig(bus, slot, function, 0x00);
    dev.deviceID = readConfig(bus, slot, function, 0x02);
    uint16_t _revid_progif = readConfig(bus, slot, function, 0x08);
    dev.revisionID = _revid_progif;
    dev.progIF = _revid_progif >> 8;
    uint16_t _subclass_classcode = readConfig(bus, slot, function, 0x0A);
    dev.subclass = _subclass_classcode;
    dev.classCode = _subclass_classcode >> 8;
    uint16_t _cls_ltm = readConfig(bus, slot, function, 0x0C);
    dev.cacheLineSize = _cls_ltm;
    dev.latencyTimer = _cls_ltm >> 8;
    uint16_t _hdt_bist = readConfig(bus, slot, function, 0x0E);
    dev.headerType = _hdt_bist;
    dev.bist = _hdt_bist >> 8;
    dev.vendorShortName = getVendorShortName(dev.vendorID);
    dev.vendorFullName = getVendorFullName(dev.vendorID);
    dev.deviceChip = getDeviceName(dev.vendorID, dev.deviceID);
    dev.deviceChipDesc = getDeviceDescription(dev.vendorID, dev.deviceID);
    if (dev.headerType == 0x00) {
        uint32_t _BAR0 = readConfig(bus, slot, function, 0x10);
        _BAR0 |= readConfig(bus, slot, function, 0x12) << 16;
        dev.standardHeader.BAR0 = _BAR0;
        uint32_t _BAR1 = readConfig(bus, slot, function, 0x14);
        _BAR1 |= readConfig(bus, slot, function, 0x16) << 16;
        dev.standardHeader.BAR1 = _BAR1;
        uint32_t _BAR2 = readConfig(bus, slot, function, 0x18);
        _BAR2 |= readConfig(bus, slot, function, 0x1A) << 16;
        dev.standardHeader.BAR2 = _BAR2;
        uint32_t _BAR3 = readConfig(bus, slot, function, 0x1C);
        _BAR3 |= readConfig(bus, slot, function, 0x1E) << 16;
        dev.standardHeader.BAR3 = _BAR3;
        uint32_t _BAR4 = readConfig(bus, slot, function, 0x20);
        _BAR4 |= readConfig(bus, slot, function, 0x22) << 16;
        dev.standardHeader.BAR4 = _BAR4;
        uint32_t _BAR5 = readConfig(bus, slot, function, 0x24);
        _BAR5 |= readConfig(bus, slot, function, 0x26) << 16;
        dev.standardHeader.BAR5 = _BAR5;
        uint32_t _CCP = readConfig(bus, slot, function, 0x28);
        _CCP |= readConfig(bus, slot, function, 0x2A) << 16;
        dev.standardHeader.cardbusCISPtr = _CCP;
        dev.standardHeader.subsysVendorID = readConfig(bus, slot, function, 0x2C);
        dev.standardHeader.subsysID = readConfig(bus, slot, function, 0x2E);
        uint32_t _ROM = readConfig(bus, slot, function, 0x30);
        _ROM |= readConfig(bus, slot, function, 0x32) << 16;
        dev.standardHeader.expROMAddr = _ROM;
        dev.standardHeader.capPtr = readConfig(bus, slot, function, 0x34);
        uint16_t _il_ip = readConfig(bus, slot, function, 0x3C);
        dev.standardHeader.intLine = _il_ip;
        dev.standardHeader.intPIN = _il_ip >> 8;
        uint16_t _mgr_mlt = readConfig(bus, slot, function, 0x3E);
        dev.standardHeader.minGrant = _mgr_mlt;
        dev.standardHeader.maxLantency = _mgr_mlt >> 8;
    }
    else if (dev.headerType == 0x01) {
        uint32_t _BAR0 = readConfig(bus, slot, function, 0x10);
        _BAR0 |= readConfig(bus, slot, function, 0x12) << 16;
        dev.pciToPciHeader.BAR0 = _BAR0;
        uint32_t _BAR1 = readConfig(bus, slot, function, 0x14);
        _BAR1 |= readConfig(bus, slot, function, 0x16) << 16;
        dev.pciToPciHeader.BAR1 = _BAR1;
        uint16_t _pbn_sbn = readConfig(bus, slot, function, 0x18);
        dev.pciToPciHeader.primBusNumber = _pbn_sbn;
        dev.pciToPciHeader.secBusNumber = _pbn_sbn >> 8;
        uint16_t _sbn_slt = readConfig(bus, slot, function, 0x1A);
        dev.pciToPciHeader.subBusNumber = _sbn_slt;
        dev.pciToPciHeader.secLatencyTimer = _sbn_slt >> 8;
        uint16_t _ib_il = readConfig(bus, slot, function, 0x1C);
        dev.pciToPciHeader.ioBase = _ib_il;
        dev.pciToPciHeader.ioLimit = _ib_il >> 8;
        dev.pciToPciHeader.secStatus = readConfig(bus, slot, function, 0x1E);
        dev.pciToPciHeader.memBase = readConfig(bus, slot, function, 0x20);
        dev.pciToPciHeader.memLimit = readConfig(bus, slot, function, 0x22);
        dev.pciToPciHeader.prefetchMemBase = readConfig(bus, slot, function, 0x24);
        dev.pciToPciHeader.prefetchMemLimit = readConfig(bus, slot, function, 0x26);
        uint32_t _PBU = readConfig(bus, slot, function, 0x28);
        _PBU |= readConfig(bus, slot, function, 0x2A) << 16;
        dev.pciToPciHeader.prefetchBaseUpper = _PBU;
        uint32_t _PLU = readConfig(bus, slot, function, 0x2C);
        _PLU |= readConfig(bus, slot, function, 0x2E) << 16;
        dev.pciToPciHeader.prefetchLimitUpper = _PLU;
        dev.pciToPciHeader.ioBaseUpper = readConfig(bus, slot, function, 0x30);
        dev.pciToPciHeader.ioLimitUpper = readConfig(bus, slot, function, 0x32);
        dev.pciToPciHeader.capPtr = readConfig(bus, slot, function, 0x34);
        uint16_t _il_ip = readConfig(bus, slot, function, 0x3C);
        dev.pciToPciHeader.intLine = _il_ip;
        dev.pciToPciHeader.intPIN = _il_ip >> 8;
    }
    else if (dev.headerType == 0x02) {  // IMPORTANT !!!! FIX THIS NEXT !!!!!!! (replace signe byte reads by one with shift)
        uint32_t _CBA = readConfig(bus, slot, function, 0x10);
        _CBA |= readConfig(bus, slot, function, 0x12) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _CBA;
        dev.pciToCardbusHeader.capListOffset = readConfig(bus, slot, function, 0x14);
        dev.pciToCardbusHeader.secStatus = readConfig(bus, slot, function, 0x16);
        dev.pciToCardbusHeader.pciBusNumber = readConfig(bus, slot, function, 0x18);
        dev.pciToCardbusHeader.cardbusBusNuber = readConfig(bus, slot, function, 0x19);
        dev.pciToCardbusHeader.subBusNumber = readConfig(bus, slot, function, 0x1A);
        dev.pciToCardbusHeader.cardbusLatTimer = readConfig(bus, slot, function, 0x1B);
        uint32_t _MBA0 = readConfig(bus, slot, function, 0x1c);
        _MBA0 |= readConfig(bus, slot, function, 0x1E) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _MBA0;
        uint32_t _ML0 = readConfig(bus, slot, function, 0x20);
        _ML0 |= readConfig(bus, slot, function, 0x22) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _ML0;
        uint32_t _MBA1 = readConfig(bus, slot, function, 0x24);
        _MBA1 |= readConfig(bus, slot, function, 0x26) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _MBA1;
        uint32_t _ML1 = readConfig(bus, slot, function, 0x28);
        _ML1 |= readConfig(bus, slot, function, 0x2A) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _ML1;
        uint32_t _IBA0 = readConfig(bus, slot, function, 0x2C);
        _IBA0 |= readConfig(bus, slot, function, 0x2E) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _IBA0;
        uint32_t _IL0 = readConfig(bus, slot, function, 0x30);
        _IL0 |= readConfig(bus, slot, function, 0x32) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _IL0;
        uint32_t _IBA1 = readConfig(bus, slot, function, 0x34);
        _IBA1 |= readConfig(bus, slot, function, 0x36) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _IBA1;
        uint32_t _IL1 = readConfig(bus, slot, function, 0x38);
        _IL1 |= readConfig(bus, slot, function, 0x3A) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _IL1;
        dev.pciToCardbusHeader.intLine = readConfig(bus, slot, function, 0x3C);
        dev.pciToCardbusHeader.intPIN = readConfig(bus, slot, function, 0x3D);
        dev.pciToCardbusHeader.subsysDeviceID = readConfig(bus, slot, function, 0x40);
        dev.pciToCardbusHeader.intPIN = readConfig(bus, slot, function, 0x42);
        uint32_t _LBA = readConfig(bus, slot, function, 0x38);
        _LBA |= readConfig(bus, slot, function, 0x3A) << 16;
        dev.pciToCardbusHeader.cardbusBaseAddr = _LBA;
    }
    return dev;
}

void PCI_Class::scanDevices() {
    _devices = (PCIDevice_t*)malloc(sizeof(PCIDevice_t));
    for (uint8_t bus = 0; bus < 16; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint16_t vendorID = PCI.getVendorID(bus, slot, function);
                if (vendorID != 0xFFFF) {
                    PCIDevice_t dev = PCI.getDevice(bus, slot, function);
                    _deviceCount++;
                    _devices = (PCIDevice_t*)realloc((void*)_devices, sizeof(PCIDevice_t) * _deviceCount);
                    _devices[_deviceCount - 1] = dev;
                }
            }
        }
    }
}

PCIDevice_t PCI_Class::getDevice(uint32_t id) {
    if (id < _deviceCount) {
        return _devices[id];
    }
    else {
        PCIDevice_t invalid;
        invalid.vendorID = 0xFFFF;
        return invalid;
    }
}

uint32_t PCI_Class::getDeviceCount() {
    return _deviceCount;
}