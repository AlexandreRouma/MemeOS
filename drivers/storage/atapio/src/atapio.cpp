#include <stdint.h>
#include <drivers/storage/atapio/atapio.h>
#include <libs/kernel/io.h>
#include <libs/kernel/panic.h>
#include <drivers/timer/pit.h>
#include <drivers/text_term/terminal.h>
#include <libs/std/math.h>

ATAPIO_Class ATAPIO;

extern "C" void __cxa_pure_virtual() { while (1); }

int selectDrive(int drive, int numblock, int count)
{
        outb(0x1F1, 0x00);	/* NULL byte to port 0x1F1 */
        outb(0x1F2, count);	/* Sector count */
        outb(0x1F3, (uint8_t) numblock);	/* Low 8 bits of the block address */
        outb(0x1F4, (uint8_t) (numblock >> 8));	/* Next 8 bits of the block address */
        outb(0x1F5, (uint8_t) (numblock >> 16));	/* Next 8 bits of the block address */

        /* Drive indicator, magic bits, and highest 4 bits of the block address */
        outb(0x1F6, 0xE0 | (drive << 4) | ((numblock >> 24) & 0x0F));

        return 0;
}

int ATAPIO_Class::readBlock(int drive, int numblock, int count, char *buf) {
    uint16_t tmpword;
    int idx;

    if (count == 0) {
        kernel_panic(0x1234, "Null sector count read");
    }

    selectDrive(drive, numblock, count);

    outb(0x1F7, 0x20);

	for (int i = 0; i < count; i++) {
        PIT.delay(1);
        uint8_t status = inb(0x1F7);
        while ((status & 0x80) && !(status & 0x08)) {
            status = inb(0x1F7);
        }

        for (idx = 0; idx < 256 * count; idx++) {
            tmpword = inw(0x1F0);
            buf[(idx * 2) + (i * 512)] = (uint8_t)tmpword;
            buf[(idx * 2 + 1) + (i * 512)] = (uint8_t)(tmpword >> 8);
        }
    }

	return count;
}

int ATAPIO_Class::writeBlock(int drive, int numblock, int count, char *buf) {
    uint16_t tmpword;
    int idx;

    if (count == 0) {
        kernel_panic(0x1234, "Null sector count write");
    }

    selectDrive(drive, numblock, count);
    outb(0x1F7, 0x30);

    // Wait for ready signal
    uint8_t status = (inb(0x1F7) & 0x08);
	while (!status) {
        status = (inb(0x1F7) & 0x08);
    }

    for (idx = 0; idx < 256 * count; idx++) {
            tmpword = (buf[idx * 2 + 1] << 8) | buf[idx * 2];
            outw(0x1F0, tmpword);
    }

    return count;
}

ATAPIODrive_t::ATAPIODrive_t(uint8_t id) {
    this->id = id;
}

bool ATAPIODrive_t::read(uint64_t addr, uint64_t size, uint8_t* buf) {
    uint64_t base = floor(addr / 512);
    uint64_t limit = ceil((addr + size) / 512);
    uint64_t start = addr % 512;
    uint8_t* buffer = (uint8_t*)malloc(((limit - base) + 1) * 512);
    ATAPIO.readBlock(0, base, (limit - base) + 1, (char*)buffer); // TODO: Change first param to this->id
    memmove(buf, buffer + start, size);
    free(buffer);
    return true; // TODO: Check operation worked
}

bool ATAPIODrive_t::write(uint64_t addr, uint64_t size, uint8_t* buf) {
    uint64_t base = floor((float)addr / 512.0f);
    uint64_t limit = floor((float)(addr + size) / 512.0f);
    uint64_t start = addr % 512;
    uint8_t* buffer = (uint8_t*)malloc(((limit - base) + 1) * 512);

    Terminal.println(itoa(base, 10));
    Terminal.println(itoa(limit, 10));
    Terminal.println(itoa(start, 10));
    Terminal.println(itoa((limit - base) + 1, 10));

    ATAPIO.readBlock(0, base, (limit - base) + 2, (char*)buffer); // TODO: Change first param to this->id
    memmove(buffer + start, buf, size);
    Terminal.println((char*)(buffer + start));
    ATAPIO.writeBlock(0, base, (limit - base) + 2, (char*)buffer);
    free(buffer);
    return true; // TODO: Check operation worked
}