#include <stdint.h>
#include <libs/kernel/paging.h>
#include <libs/kernel/multiboot.h>
#include <libs/kernel/panic.h>
#include <drivers/text_term/terminal.h>

Paging_Class Paging;

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_tables[1024][1024] __attribute__((aligned(4096)));

void initPageDirectory() {
    for (uint16_t i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }
}

void initPageTables() {
    for (uint16_t i = 0; i < 1024; i++) {
        for (uint16_t j = 0; j < 1024; j++) {
            page_tables[i][j] = 0x00000002;
        }
    }
}

void initPageTable() {
    for (uint32_t i = 0; i < 1024; i++) {
        for (uint32_t j = 0; j < 1024; j++) {
            page_tables[i][j] = ((j * 0x1000) + (i * 0x400000)) | 2;
        }
    }
}

void fillPageDirectory() {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = ((uint32_t)&page_tables[i][0]) | 3;
    }
}

void Paging_Class::enable() {
    initPageDirectory();
    initPageTables();
    initPageTable();
    fillPageDirectory();
    setPresent(0, ((uint32_t)ASM_KERNEL_END / 4096) + 1); // Allocate for the kernel
    ASM_LOAD_PAGE_DIRECTORY(page_directory);
    ASM_ENABLE_PAGING();
}

void Paging_Class::mapPage(uint32_t phy, uint32_t virt, uint16_t flags) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;
    page_tables[pdi][pti] = phy | flags;
}

uint32_t Paging_Class::getPhysicalAddr(uint32_t virt) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;
    return page_tables[pdi][pti] & 0xFFFFF000;
}

uint16_t Paging_Class::getFlags(uint32_t virt) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;
    return page_tables[pdi][pti] & 0xFFF;
}

void Paging_Class::setFlags(uint32_t virt, uint16_t flags) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;
    page_tables[pdi][pti] &= 0xFFFFF000;
    page_tables[pdi][pti] |= flags;
}

uint16_t Paging_Class::getDirectoryFlags(uint32_t virt) {
    uint32_t pdi = virt >> 22;
    return page_directory[pdi] & 0xFFF;
}

void Paging_Class::setDirectoryFlags(uint32_t virt, uint16_t flags) {
    uint32_t pdi = virt >> 22;
    page_directory[pdi] &= 0xFFFFF000;
    page_directory[pdi] |= flags;
}

void Paging_Class::setPresent(uint32_t virt, uint32_t count) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;

    uint32_t usedCount = 0;

    for (uint32_t i = pdi; i < 1024; i++) {
        for (uint32_t j = pti; j < 1024; j++) {
            page_tables[i][j] |= 1;
            usedCount++;
            if (usedCount >= count) {
                return;
            }
        }
    }
}

void Paging_Class::setAbsent(uint32_t virt, uint32_t count) {
    uint32_t pdi = virt >> 22;
    uint32_t pti = virt >> 12 & 0x03FF;

    uint32_t usedCount = 0;

    for (uint32_t i = pdi; i < 1024; i++) {
        for (uint32_t j = pti; j < 1024; j++) {
            page_tables[i][j] &= 0xFFFFFFFE;
            usedCount++;
            if (usedCount >= count) {
                return;
            }
        }
    }
}

uint32_t Paging_Class::findPages(uint32_t count) {
    uint32_t continous = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        for (uint32_t j = 0; j < 1024; j++) {
            uint8_t free = !(page_tables[i][j] & 1);
            if (free == 1) {
                continous++;
            }
            else {
                continous = 0;
            }
            if (continous == count) {
                return (i * 0x400000) + (j * 0x1000);
            }
        }
    }
    kernel_panic(0xF0CC, "Out of memory :/");
    return 0;
}

uint32_t Paging_Class::allocPages(uint32_t count) {
    uint32_t ptr = findPages(count);
    setPresent(ptr, count);
    return ptr;
}

uint32_t Paging_Class::getUsedPages() {
    uint32_t n = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        for (uint32_t j = 0; j < 1024; j++) {
            uint8_t flags = page_tables[i][j] & 0x01;
            if (flags == 1) {
                n++;
            }
        }
    }
    return n;
}