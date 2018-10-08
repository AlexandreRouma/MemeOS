#define EXT2_STATE_CLEAN    1
#define EXT2_STATE_ERROR    2

#define EXT2_ERR_IGNORE     1
#define EXT2_ERR_REMOUNT    2
#define EXT2_ERR_PANIC      3

#define EXT2_CREATOR_LINUX  0
#define EXT2_CREATOR_HURD   1
#define EXT2_CREATOR_MASIX  2
#define EXT2_CREATOR_FRBSD  3
#define EXT2_CREATOR_BSD    4

#define EXT2_OFF_PREALLOC   0x01
#define EXT2_OFF_AFS        0x02
#define EXT2_OFF_JOURNAL    0x04
#define EXT2_OFF_EXT_ATT    0x08
#define EXT2_OFF_RESIZE     0x10
#define EXT2_OFF_HASH_IND   0x20

#define EXT2_RFF_COMPRESS   0x01
#define EXT2_RFF_TYPE_FLD   0x02
#define EXT2_RFF_REP_JRNL   0x04
#define EXT2_RFF_JRNL_DEV   0x08

#define EXT2_ROFF_SPARSE    0x01
#define EXT2_ROFF_64BIT     0x02
#define EXT2_ROFF_BINTREE   0x04

struct Ext2BaseSuperBlock_t {
    uint32_t totalInode;
    uint32_t totalBlock;
    uint32_t suResBlocks;
    uint32_t totalUnallocInode;
    uint32_t totalUnallocBlock;
    uint32_t sbBlockID;
    uint32_t log2BlockSize;
    uint32_t log2FragmentSize;
    uint32_t blockPerGroup;
    uint32_t fragmentPerGroup;
    uint32_t inodePerGroup;
    uint32_t lastMountTime;
    uint32_t lastWriteTime;
    uint16_t mountCountSinceCC;
    uint16_t maxMountCount;
    uint16_t ext2Signature;
    uint16_t fsState;
    uint16_t errorFunction;
    uint16_t versionMin;
    uint32_t lastCCTime;
    uint32_t ccInterval;
    uint32_t osID;
    uint32_t versionMaj;
    uint16_t blockResUsrID;
    uint16_t blockResGrpID;
}__attribute__((packed));

struct Ext2ExtendedSuperBlock_t {
    uint32_t firstNonReservedInode;
    uint16_t inodeSize;
    uint16_t blockGround;
    uint32_t optFeatures;
    uint32_t reqFeatures;
    uint32_t nonSupportedFeatures;
    char fileSystemID[16];
    char volumeName[16];
    char lastMountPath[64];
    uint32_t compAlgorithm;
    char filePreAllocBlockCount;
    char dirPreAllocBlockCount;
    uint16_t _unused;
    char journalID[16];
    uint32_t journalInode;
    uint32_t journalDevice;
    uint32_t orphanInodeHead;
    bool present;
}__attribute__((packed));

struct Ext2BlockGroupDeskEntry_t {
    uint32_t blockUsageBmpAddr;
    uint32_t inodeUsageBmpAddr;
    uint32_t inodeStartBlockAddr;
    uint16_t unAllocBlockCount;
    uint16_t unAllocInodeCount;
    uint16_t unAllocDirCount;
    char unused[14];
}__attribute__((packed));

struct Ext2Inode_t {
    uint16_t typeNPerms;
    uint16_t userID;
    uint32_t size_l;
    uint32_t lastAccessTime;
    uint32_t creationTime;
    uint32_t lastModTime;
    uint32_t deletionTime;
    uint16_t groupID;
    uint16_t hardLinkCount;
    uint32_t diskSectorCount;
    uint32_t flags;
    uint32_t osSpecific1;
    uint32_t directBlockPointer0;
    uint32_t directBlockPointer1;
    uint32_t directBlockPointer2;
    uint32_t directBlockPointer3;
    uint32_t directBlockPointer4;
    uint32_t directBlockPointer5;
    uint32_t directBlockPointer6;
    uint32_t directBlockPointer7;
    uint32_t directBlockPointer8;
    uint32_t directBlockPointer9;
    uint32_t directBlockPointer10;
    uint32_t directBlockPointer11;
    uint32_t singlyBlockPointer;
    uint32_t doublyBlockPointer;
    uint32_t triplyBlockPointer;
    uint32_t generationNumber;
    uint32_t res_extAttrib;
    uint32_t res_size_h;
    uint32_t fragmentBlockAddr;
    char osSpecific2[12];
}__attribute__((packed));

void parseFs();