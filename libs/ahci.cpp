#include <stdint.h>
#include <ahci.h>
#include <pci.h>
#include <liballoc.h>
#include <terminal.h>
#include <string.h>

AHCI_Class AHCI;

void AHCI_Class::scanControllers() {
    _ahciControllers = (ACHIController_t*)malloc(sizeof(ACHIController_t));
    for (uint8_t id = 0; id < PCI.getDeviceCount(); id++) {
        PCIDevice_t dev = PCI.getDevice(id);
        if (dev.classCode == 0x01 && dev.subclass == 0x06) {
            ACHIController_t cnt;
            cnt.pciDevice = dev;
            _controllerCount++;
            _ahciControllers = (ACHIController_t*)realloc(_ahciControllers, sizeof(ACHIController_t) * _controllerCount);
            _ahciControllers[_controllerCount - 1] = cnt;
        }
    }
}

uint32_t AHCI_Class::getControllerCount() {
    return _controllerCount;
}

ACHIController_t AHCI_Class::getController(uint32_t id) {
    if (id < _controllerCount) {
        return _ahciControllers[id];
    }
    else {
        ACHIController_t invalid;
        invalid.pciDevice.vendorID = 0xFFFF;
        return invalid;
    }
}