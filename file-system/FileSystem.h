//
// Created by Oguzhan Agkus on 16.05.2022.
//

#ifndef FILE_SYSTEM_FILESYSTEM_H
#define FILE_SYSTEM_FILESYSTEM_H

#define BLOCKS_PER_INODE 4
#define REGULAR_FILE 1
#define DIRECTORY 2

#include <string>
#include <cstring>
#include <ctime>
#include <cstdint>
#include <utility>
#include <fstream>

struct SuperBlock {
    uint32_t diskSize;
    uint32_t blockSize;
    uint32_t blockCount;
    uint32_t freeBlockCount;
    uint32_t firstBlock;
    uint32_t inodeSize;
    uint32_t inodeCount;
    uint32_t freeInodeCount;
    uint32_t firstInode;
    uint32_t inodeBitmapBlockCount;
    uint32_t blockBitmapBlockCount;
    uint32_t inodeBlockCount;
    uint32_t dataBlockCount;
    uint32_t inodeBitmapStartBlock;
    uint32_t blockBitmapStartBlock;
    uint32_t inodeStartBlock;
    uint32_t dataStartBlock;
    uint32_t rootInode;
    uint32_t directoryCount;
    uint32_t fileCount;
};

struct Inode {
    uint8_t mode;
    uint8_t blockCount;
    uint16_t blocks[7];
    uint32_t size;
    uint32_t creationTime;
    uint32_t lastAccessTime;
    uint32_t lastModificationTime;
};

struct DirectoryEntry {
    uint16_t inodeNumber;
    unsigned char name[14];
};

class FileSystem {
public:
    explicit FileSystem(std::string diskFile);

    ~FileSystem();

    bool create(unsigned short _diskSize_, unsigned _blockSize_);

    bool load();

    void listDirectory(char *path);

    void makeDirectory(char *path);

    void removeDirectory(char *path);

    void dumpe2fs() const;

    void writeFile(char *path, char *file);

    void readFile(char *path, char *file);

    void removeFile(char *path);

private:
    std::fstream disk;
    std::string diskFile;
    SuperBlock superBlock{};
    bool *inodeBitmap = nullptr;
    bool *blockBitmap = nullptr;
    bool initialized = false;

    int allocateInode();

    int allocateBlock();

    void readInode(uint16_t inodeNumber, Inode *inode);

    void writeInode(uint16_t inodeNumber, Inode *inode);

    void readBlock(uint16_t blockNumber, char *block);

    void writeBlock(uint16_t blockNumber, char *block);

    void readSuperBlock();

    void readInodeBitmap();

    void readBlockBitmap();

    void writeSuperBlock();

    void writeInodeBitmap();

    void writeBlockBitmap();
};

#endif //FILE_SYSTEM_FILESYSTEM_H
