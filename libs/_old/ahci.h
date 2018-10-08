#include <stdint.h>
#include <pci.h>

#define FIS_TYPE_REG_H2D	0x27    // Register FIS - host to device
#define FIS_TYPE_REG_D2H	0x34	// Register FIS - device to host
#define FIS_TYPE_DMA_ACT	0x39	// DMA activate FIS - device to host
#define FIS_TYPE_DMA_SETUP	0x41	// DMA setup FIS - bidirectional
#define FIS_TYPE_DATA		0x46	// Data FIS - bidirectional
#define FIS_TYPE_BIST		0x58	// BIST activate FIS - bidirectional
#define FIS_TYPE_PIO_SETUP	0x5F	// PIO setup FIS - device to host
#define FIS_TYPE_DEV_BITS	0xA1	// Set device bits FIS - device to host

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM		0x96690101	// Port multiplier
 
#define AHCI_DEV_NULL 	0
#define AHCI_DEV_SATA 	1
#define AHCI_DEV_SEMB	2
#define AHCI_DEV_PM 	3
#define AHCI_DEV_SATAPI 4
 
#define HBA_PORT_IPM_ACTIVE		1
#define HBA_PORT_DET_PRESENT	3

#define ATA_DEV_BUSY	0x80
#define ATA_DEV_DRQ		0x08

struct FIS_REG_H2D_t {
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_REG_H2D
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:3;		// Reserved
	uint8_t  c:1;		// 1: Command, 0: Control
 
	uint8_t  command;	// Command register
	uint8_t  featurel;	// Feature register, 7:0
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  featureh;	// Feature register, 15:8
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  icc;		// Isochronous command completion
	uint8_t  control;	// Control register
 
	// DWORD 4
	uint8_t  rsv1[4];	// Reserved
};

struct FIS_REG_D2H_t {
	// DWORD 0
	uint8_t  fis_type;    // FIS_TYPE_REG_D2H
 
	uint8_t  pmport:4;    // Port multiplier
	uint8_t  rsv0:2;      // Reserved
	uint8_t  i:1;         // Interrupt bit
	uint8_t  rsv1:1;      // Reserved
 
	uint8_t  status;      // Status register
	uint8_t  error;       // Error register
 
	// DWORD 1
	uint8_t  lba0;        // LBA low register, 7:0
	uint8_t  lba1;        // LBA mid register, 15:8
	uint8_t  lba2;        // LBA high register, 23:16
	uint8_t  device;      // Device register
 
	// DWORD 2
	uint8_t  lba3;        // LBA register, 31:24
	uint8_t  lba4;        // LBA register, 39:32
	uint8_t  lba5;        // LBA register, 47:40
	uint8_t  rsv2;        // Reserved
 
	// DWORD 3
	uint8_t  countl;      // Count register, 7:0
	uint8_t  counth;      // Count register, 15:8
	uint8_t  rsv3[2];     // Reserved
 
	// DWORD 4
	uint8_t  rsv4[4];     // Reserved
};

struct FIS_DATA_t {
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_DATA
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:4;		// Reserved
 
	uint8_t  rsv1[2];	// Reserved
 
	// DWORD 1 ~ N
	uint32_t data[1];	// Payload
};

struct FIS_PIO_SETUP_t {
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_PIO_SETUP
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:1;		// Reserved
	uint8_t  d:1;		// Data transfer direction, 1 - device to host
	uint8_t  i:1;		// Interrupt bit
	uint8_t  rsv1:1;
 
	uint8_t  status;		// Status register
	uint8_t  error;		// Error register
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  rsv2;		// Reserved
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  rsv3;		// Reserved
	uint8_t  e_status;	// New value of status register
 
	// DWORD 4
	uint16_t tc;		// Transfer count
	uint8_t  rsv4[2];	// Reserved
};

struct FIS_DMA_SETUP_t {
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_DMA_SETUP
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:1;		// Reserved
	uint8_t  d:1;		// Data transfer direction, 1 - device to host
	uint8_t  i:1;		// Interrupt bit
	uint8_t  a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed
 
    uint8_t  rsved[2];       // Reserved

    //DWORD 1&2

    uint64_t DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

    //DWORD 3
    uint32_t rsvd;           //More reserved

    //DWORD 4
    uint32_t DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0

    //DWORD 5
    uint32_t TransferCount;  //Number of bytes to transfer. Bit 0 must be 0

    //DWORD 6
    uint32_t resvd;          //Reserved
};

struct HBAPrdtEntry_t {
	uint32_t dba;
	uint32_t dbau;
	uint32_t res0;
	uint32_t dbc:22;
	uint32_t rsv1:9;
	uint32_t i:1;
};

struct HBAReceivedFIS_t {
	FIS_DMA_SETUP_t dmaSetupFIS;
	uint32_t unused0;
	FIS_PIO_SETUP_t pioSetupFIS;
	uint8_t unused1[12];
	FIS_REG_D2H_t registerFIS;
	uint32_t unused2;
	uint64_t setDeviceBitFIS;
	uint8_t uFIS[0x40];
	uint8_t reserved[0x5F];
};

struct HBACommandTable_t {
	FIS_REG_H2D_t commandFIS;
	uint8_t atapiCmd[0x10];
	uint8_t res0[0x20];
	HBAPrdtEntry_t* prdt;
};

struct HBACommand_t {
	uint8_t  cfl:5;
	uint8_t  a:1;
	uint8_t  w:1;
	uint8_t  p:1;
	uint8_t  r:1;
	uint8_t  b:1;
	uint8_t  c:1;
	uint8_t  rsv0:1;
	uint8_t  pmp:4;
	uint16_t prdtl;
	uint32_t prdbc;
	uint8_t res0;
	uint8_t ctba0_l;
	uint16_t ctba0_h;
	uint32_t ctba_u0;
	uint32_t res1[0x04];
};

struct HBAPort_t {
    uint32_t clb;		// 0x00, command list base address, 1K-byte aligned
	uint32_t clbu;		// 0x04, command list base address upper 32 bits
	uint32_t fb;		// 0x08, FIS base address, 256-byte aligned
	uint32_t fbu;		// 0x0C, FIS base address upper 32 bits
	uint32_t is;		// 0x10, interrupt status
	uint32_t ie;		// 0x14, interrupt enable
	uint32_t cmd;		// 0x18, command and status
	uint32_t rsv0;		// 0x1C, Reserved
	uint32_t tfd;		// 0x20, task file data
	uint32_t sig;		// 0x24, signature
	uint32_t ssts;		// 0x28, SATA status (SCR0:SStatus)
	uint32_t sctl;		// 0x2C, SATA control (SCR2:SControl)
	uint32_t serr;		// 0x30, SATA error (SCR1:SError)
	uint32_t sact;		// 0x34, SATA active (SCR3:SActive)
	uint32_t ci;		// 0x38, command issue
	uint32_t sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	uint32_t fbs;		// 0x40, FIS-based switch control
	uint32_t rsv1[0x0B];	// 0x44 ~ 0x6F, Reserved
	uint32_t vendor[0x04];	// 0x70 ~ 0x7F, vendor specific
};

struct HBAMem_t {
    // 0x00 - 0x2B, Generic Host Control
	uint32_t cap;		// 0x00, Host capability
	uint32_t ghc;		// 0x04, Global host control
	uint32_t is;		// 0x08, Interrupt status
	uint32_t pi;		// 0x0C, Port implemented
	uint32_t vs;		// 0x10, Version
	uint32_t ccc_ctl;	// 0x14, Command completion coalescing control
	uint32_t ccc_pts;	// 0x18, Command completion coalescing ports
	uint32_t em_loc;		// 0x1C, Enclosure management location
	uint32_t em_ctl;		// 0x20, Enclosure management control
	uint32_t cap2;		// 0x24, Host capabilities extended
	uint32_t bohc;		// 0x28, BIOS/OS handoff control and status

    uint8_t _reserved[0x74];
    uint8_t _vendorspc[0x60];

    HBAPort_t ports[0x20];
};

struct ACHIDrive_t {
    uint8_t type;
    HBAPort_t* port;
    uint32_t controllerID;
};

struct ACHIController_t {
    PCIDevice_t pciDevice;
    HBAMem_t* hbaMem;
    ACHIDrive_t* drives;
    uint8_t driveCount;
};

class AHCI_Class {
    public:
        void scanControllers();
        uint32_t getControllerCount();
        ACHIController_t getController(uint32_t id);

    private:
        ACHIController_t* _ahciControllers;
        uint32_t _controllerCount;
};

extern AHCI_Class AHCI;

