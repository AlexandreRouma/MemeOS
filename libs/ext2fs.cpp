#include <stdint.h>
#include <ext2fs.h>
#include <atapio.h>
#include <liballoc.h>
#include <string.h>
#include <panic.h>
#include <terminal.h>
#include <pit.h>

Ext2BaseSuperBlock_t _baseSuperBLock;
Ext2ExtendedSuperBlock_t _extSuperBlock;
uint32_t blockGroupCount = 0;
uint32_t blockSize;
uint32_t fragmentSize;
uint32_t inodeSize;
Ext2BlockGroupDeskEntry_t* _blockGroupDescriptorTable;
char* buffer;

Ext2BaseSuperBlock_t parseBaseSuperblock() {
    Ext2BaseSuperBlock_t ret;
    buffer = (char*)malloc(1024);
    ATAPIO.readBlock(0, 2, 2, buffer);
    memmove(&ret, buffer, sizeof(Ext2BaseSuperBlock_t));
    free(buffer);
    return ret;
}

Ext2ExtendedSuperBlock_t parseExtSuperblock() {
    Ext2ExtendedSuperBlock_t ret;
    char* buffer = (char*)malloc(1024);
    //ATAPIO.readBlock(0, 2, 2, buffer);
    //PIT.delay(1000);
    ATAPIO.readBlock(0, 2, 2, buffer);
    memmove(&ret, buffer + 84, sizeof(Ext2ExtendedSuperBlock_t));
    free(buffer);
    return ret;
}

uint32_t getPhysicalBlock(uint32_t block) {
    return (blockSize / 512) * block;
}

uint32_t getBlockCount(uint32_t block) {
    return (blockSize / 512) * block;
}

void parseBlockGroupDescriptorTable() {
    char* buffer = (char*)malloc(blockSize); // 1 block
    if (blockSize == 1024) {
        ATAPIO.readBlock(0, getPhysicalBlock(2), getBlockCount(1), buffer);
    }
    else {
        ATAPIO.readBlock(0, getPhysicalBlock(1), getBlockCount(1), buffer);
    }
    _blockGroupDescriptorTable = (Ext2BlockGroupDeskEntry_t*)malloc(sizeof(Ext2BlockGroupDeskEntry_t) * blockGroupCount);
    memmove(_blockGroupDescriptorTable, buffer,  sizeof(Ext2BlockGroupDeskEntry_t) * blockGroupCount);
    free(buffer);
}

uint32_t getBlockGroup(uint32_t inode) {
    return (inode - 1) / _baseSuperBLock.inodePerGroup;
}

uint32_t getInodeIndex(uint32_t inode) {
    return (inode - 1) % _baseSuperBLock.inodePerGroup;
}

uint32_t getInodeBlock(uint32_t index) {
    return (index * inodeSize) / blockSize;
}

void parseFs() {
    _baseSuperBLock = parseBaseSuperblock();
    if (_baseSuperBLock.ext2Signature != 0xEF53) {
        Terminal.setColor(0x04);
        Terminal.println("Invalid filesystem");
        Terminal.setColor(0x07);
        return;
    }
    if (_baseSuperBLock.versionMaj >= 1) {
        
    }
    _extSuperBlock = parseExtSuperblock();
    _extSuperBlock.present = true;
    uint32_t blockGroupCount_b  = _baseSuperBLock.totalBlock / _baseSuperBLock.blockPerGroup;
    uint32_t blockGroupCount_i  = _baseSuperBLock.totalInode / _baseSuperBLock.inodePerGroup;
    if (blockGroupCount_b != blockGroupCount_i) {
        Terminal.setColor(0x04);
        Terminal.println("Invalid block group count");
        Terminal.setColor(0x07);
        return;
    }
    blockGroupCount = blockGroupCount_b;
    blockSize = 1024 << _baseSuperBLock.log2BlockSize;
    fragmentSize = 1024 << _baseSuperBLock.log2FragmentSize;
    if (_baseSuperBLock.versionMaj >= 1) {
        inodeSize = _extSuperBlock.inodeSize;
    }
    else {
        inodeSize = 128;
    }
    parseBlockGroupDescriptorTable();

    uint32_t blockGroup = getBlockGroup(13);
    uint32_t inodeTableAddr = _blockGroupDescriptorTable[blockGroup].inodeStartBlockAddr;
    uint32_t inodeIndex = getInodeIndex(13);
    if (_extSuperBlock.present) {
        Terminal.println("Present");
    }
    Terminal.println(itoa(_extSuperBlock.firstNonReservedInode, 10));
}