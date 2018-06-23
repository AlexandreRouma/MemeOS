#include <stdint.h>
#include <paging.h>

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
            page_tables[i][j] = ((j * 0x1000) + (i * 0x400000)) | 3;
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