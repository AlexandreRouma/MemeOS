#include <stdint.h>
#include <atapio.h>
#include <io.h>
#include <panic.h>
#include <pit.h>
#include <terminal.h>

ATAPIO_Class ATAPIO;

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

    Terminal.print("Waiting for drive ready... ");
    inb(0x1F7);
    inb(0x1F7);
    inb(0x1F7);
    inb(0x1F7);
    uint8_t status = inb(0x1F7);
	while ((status & 0x80) && !(status & 0x08)) {
        status = inb(0x1F7);
    }
    Terminal.println("READY");

	selectDrive(drive, numblock, count);

	outb(0x1F7, 0x20);

    // Wait for ready signal
    Terminal.print("Waiting for data... ");
    inb(0x1F7);
    inb(0x1F7);
    inb(0x1F7);
    inb(0x1F7);
    status = inb(0x1F7);
	while ((status & 0x80) && !(status & 0x08)) {
        status = inb(0x1F7);
    }
    Terminal.println("READING");

	for (idx = 0; idx < 256 * count; idx++) {
		tmpword = inw(0x1F0);
		buf[idx * 2] = (uint8_t)tmpword;
		buf[idx * 2 + 1] = (uint8_t)(tmpword >> 8);
	}

    outb(0x1F0, 0x04); // Reset

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