#include <stdint.h>
#include <ahci.h>
#include <pci.h>
#include <liballoc.h>
#include <terminal.h>
#include <string.h>
#include <paging.h>

AHCI_Class AHCI;

int getCommandSlot(HBAPort_t* port) {
	for (int i = 0; i < 32; i++) {
		if ((port->ci >> i) & 1 == 0) {
			port->ci |= 1 << i;
			return i;
		}
	}
	return -1;
}

uint8_t checkPort(HBAPort_t* port) {
    uint32_t ssts = port->ssts;
    uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;

    if (det != HBA_PORT_DET_PRESENT || ipm != HBA_PORT_IPM_ACTIVE) {
        return AHCI_DEV_NULL;
    }

    switch (port->sig)
	{
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
	}
}

void AHCI_Class::scanControllers() {
    _ahciControllers = (ACHIController_t*)malloc(sizeof(ACHIController_t));
    for (uint8_t id = 0; id < PCI.getDeviceCount(); id++) {
        PCIDevice_t dev = PCI.getDevice(id);
        if (dev.classCode == 0x01 && dev.subclass == 0x06) {
            ACHIController_t cnt;
            cnt.pciDevice = dev;
            cnt.drives = (ACHIDrive_t*)malloc(sizeof(ACHIDrive_t));
            Paging.setPresent(dev.standardHeader.BAR5, (sizeof(HBAMem_t) / 4096) + 1);
            cnt.hbaMem = (HBAMem_t*)dev.standardHeader.BAR5;
            for (uint8_t i = 0; i < 32; i++) {
                if ((cnt.hbaMem->pi >> i) & 1 == 1) {
                    uint8_t drvType = checkPort(&cnt.hbaMem->ports[i]);
                    if (drvType != AHCI_DEV_NULL) {
                       
                        ACHIDrive_t drv;
                        drv.port = &cnt.hbaMem->ports[i];
                        drv.type = drvType;
                        drv.controllerID = _controllerCount;
                        cnt.driveCount++;
                        cnt.drives = (ACHIDrive_t*)realloc(cnt.drives, sizeof(ACHIDrive_t) * cnt.driveCount);
                        cnt.drives[cnt.driveCount - 1] = drv;
                        Terminal.println(itoa(getCommandSlot(&cnt.hbaMem->ports[i]), 16));
                    }
                }
            }
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