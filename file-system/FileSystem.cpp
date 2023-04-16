//
// Created by Oguzhan Agkus on 16.05.2022.
//

#include "Helper.h"
#include "FileSystem.h"

FileSystem::FileSystem(std::string _diskFile_) {
    diskFile = move(_diskFile_);
}

FileSystem::~FileSystem() {
    writeInodeBitmap();
    writeBlockBitmap();
    delete blockBitmap;
    delete inodeBitmap;
    disk.close();
}

bool FileSystem::create(unsigned short _diskSize_, unsigned int _blockSize_) {
    // Super block
    superBlock.diskSize = _diskSize_ * (1 << 20);
    superBlock.blockSize = _blockSize_ * (1 << 10);
    superBlock.blockCount = superBlock.diskSize / superBlock.blockSize;
    superBlock.freeBlockCount = superBlock.blockCount;
    superBlock.firstBlock = 0;
    superBlock.inodeSize = sizeof(Inode);
    superBlock.inodeCount = superBlock.freeBlockCount / BLOCKS_PER_INODE;
    superBlock.freeInodeCount = superBlock.inodeCount;
    superBlock.firstInode = 0;
    superBlock.inodeBitmapBlockCount = divide_ceil(superBlock.inodeCount, superBlock.blockSize);
    superBlock.blockBitmapBlockCount = divide_ceil(superBlock.blockCount, superBlock.blockSize);
    superBlock.inodeBlockCount = divide_ceil(superBlock.inodeCount * superBlock.inodeSize,
                                             superBlock.blockSize);
    superBlock.dataBlockCount = superBlock.blockCount - superBlock.inodeBitmapBlockCount -
                                superBlock.blockBitmapBlockCount - superBlock.inodeBlockCount - 1;
    superBlock.inodeBitmapStartBlock = 1;
    superBlock.blockBitmapStartBlock = superBlock.inodeBitmapStartBlock + superBlock.inodeBitmapBlockCount;
    superBlock.inodeStartBlock = superBlock.blockBitmapStartBlock + superBlock.blockBitmapBlockCount;
    superBlock.dataStartBlock = superBlock.inodeStartBlock + superBlock.inodeBlockCount;
    superBlock.freeBlockCount = superBlock.dataBlockCount;
    superBlock.inodeCount = superBlock.freeBlockCount / BLOCKS_PER_INODE;
    superBlock.freeInodeCount = superBlock.inodeCount;
    superBlock.firstBlock = superBlock.dataStartBlock;

    // Create bitmaps for free space management
    inodeBitmap = new bool[superBlock.inodeCount];
    blockBitmap = new bool[superBlock.blockCount];

    for (int i = 0; i < superBlock.inodeCount; i++) {
        inodeBitmap[i] = false;
    }
    for (int i = 0; i < superBlock.blockCount; i++) {
        blockBitmap[i] = false;
    }
    for (int i = 0; i < superBlock.dataStartBlock; i++) {
        blockBitmap[i] = true;
    }

    superBlock.rootInode = allocateInode();
    Inode rootInode{};
    rootInode.mode = DIRECTORY;
    rootInode.blockCount = 1;
    rootInode.blocks[0] = allocateBlock();
    rootInode.size = superBlock.blockSize;
    rootInode.creationTime = std::time(nullptr);
    rootInode.lastAccessTime = std::time(nullptr);
    rootInode.lastModificationTime = std::time(nullptr);

    // Resize file and then write required information and initial data to file
    disk.open(diskFile, std::ios::out | std::ios::binary | std::ios::trunc);
    if (errno == 0) {
        disk.seekg(superBlock.diskSize - 1, std::ios::beg);
        disk.write("\0", 1);

        writeSuperBlock();
        writeInodeBitmap();
        writeBlockBitmap();
        writeInode(superBlock.rootInode, &rootInode);

        initialized = true;
    }

    return initialized;
}

bool FileSystem::load() {
    disk.open(diskFile, std::ios::out | std::ios::in | std::ios::binary);
    if (errno == 0) {
        readSuperBlock();

        inodeBitmap = new bool[superBlock.inodeCount];
        blockBitmap = new bool[superBlock.blockCount];

        readInodeBitmap();
        readBlockBitmap();

        initialized = true;
    }

    return initialized;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"

void FileSystem::listDirectory(char *directory) {

}

void FileSystem::makeDirectory(char *path) {
    uint16_t directoryEntryCount = superBlock.blockSize / sizeof(DirectoryEntry);
    DirectoryEntry directoryEntries[directoryEntryCount];

    Inode currentInode{};
    readInode(superBlock.rootInode, &currentInode);

    bool found = true;
    const char delim[2] = "/";
    char *current, *next;

    current = strtok(path, delim);
    next = strtok(nullptr, delim);
    while (next != nullptr) {
        std::cout << current << " " << next << std::endl;
        found = false;
        for (uint8_t i = 0; i < currentInode.blockCount; i++) {
            readBlock(currentInode.blocks[i], (char *) &directoryEntries);
            for (uint16_t j = 0; j < directoryEntryCount; j++) {
                //std::cout << directoryEntries[j].inodeNumber << " - " << directoryEntries[j].name << std::endl;
                if (strcmp((const char *) (directoryEntries[j].name), current) == 0) {
                    found = true;
                    current = next;
                    next = strtok(nullptr, delim);
                    if (next) {
                        readInode(directoryEntries[j].inodeNumber, &currentInode);
                    }
                    break;
                }
            }
            if (found) break;
        }
        if (!found) break;
    }

    if (!found) return;

    std::cout << current;

}

void FileSystem::removeDirectory(char *directory) {

}

void FileSystem::dumpe2fs() const {
    std::cout <<
              "Disk size: " << superBlock.diskSize << " bytes" << std::endl <<
              "Block size: " << superBlock.blockSize << " bytes" << std::endl <<
              "Block count: " << superBlock.blockCount << std::endl <<
              "Free block count: " << superBlock.freeBlockCount << std::endl <<
              "First block: " << superBlock.firstBlock << std::endl <<
              "Inode size: " << superBlock.inodeSize << " bytes" << std::endl <<
              "Inode count: " << superBlock.inodeCount << std::endl <<
              "Free inode count: " << superBlock.freeInodeCount << std::endl <<
              "First inode: " << superBlock.firstInode << std::endl <<
              "Inode bitmap block count: " << superBlock.inodeBitmapBlockCount << std::endl <<
              "Block bitmap block count: " << superBlock.blockBitmapBlockCount << std::endl <<
              "Inode block count: " << superBlock.inodeBlockCount << std::endl <<
              "Data block count: " << superBlock.dataBlockCount << std::endl <<
              "Inode bitmap start block: " << superBlock.inodeBitmapStartBlock << std::endl <<
              "Block bitmap start block: " << superBlock.blockBitmapStartBlock << std::endl <<
              "Inode start block: " << superBlock.inodeStartBlock << std::endl <<
              "Data start block: " << superBlock.dataStartBlock << std::endl <<
              "Root inode: " << superBlock.rootInode << std::endl;

    std::cout << "Used inodes: ";
    for (int i = 0; i < superBlock.inodeCount; i++) {
        if (inodeBitmap[i]) { std::cout << i << " "; }
    }
    std::cout << std::endl;
    std::cout << "Free inodes: ";
    for (int i = 0; i < superBlock.inodeCount; i++) {
        if (!inodeBitmap[i]) { std::cout << i << " "; }
    }
    std::cout << std::endl;
    std::cout << "Used blocks: ";
    for (int i = 0; i < superBlock.blockCount; i++) {
        if (blockBitmap[i]) { std::cout << i << " "; }
    }
    std::cout << std::endl;
    std::cout << "Free blocks: ";
    for (int i = 0; i < superBlock.blockCount; i++) {
        if (!blockBitmap[i]) { std::cout << i << " "; }
    }
    std::cout << std::endl;

    // TODO: Traverse over all directories and show used blocks
}

void FileSystem::writeFile(char *path, char *file) {

}

void FileSystem::readFile(char *path, char *file) {

}

void FileSystem::removeFile(char *path) {

}

#pragma clang diagnostic pop

int FileSystem::allocateInode() {
    for (int i = 0; i < superBlock.inodeCount; i++) {
        if (!inodeBitmap[i]) {
            inodeBitmap[i] = true;
            return i;
        }
    }
    return -1;
}

int FileSystem::allocateBlock() {
    for (int i = 0; i < superBlock.blockCount; i++) {
        if (!blockBitmap[i]) {
            blockBitmap[i] = true;
            return i;
        }
    }
    return -1;
}

void FileSystem::readInode(uint16_t inodeNumber, Inode *inode) {
    uint32_t index = superBlock.blockSize * superBlock.inodeStartBlock + superBlock.inodeSize * inodeNumber;

    disk.seekg(index, std::ios::beg);
    disk.read((char *) inode, superBlock.inodeSize);
}

void FileSystem::writeInode(uint16_t inodeNumber, Inode *inode) {
    uint32_t index = superBlock.blockSize * superBlock.inodeStartBlock + superBlock.inodeSize * inodeNumber;

    disk.seekg(index, std::ios::beg);
    disk.write((char *) inode, superBlock.inodeSize);
}

void FileSystem::readBlock(uint16_t blockNumber, char *block) {
    uint32_t index = superBlock.blockSize * superBlock.dataStartBlock + superBlock.blockSize * blockNumber;

    disk.seekg(index, std::ios::beg);
    disk.read((char *) block, superBlock.blockSize);
}

void FileSystem::writeBlock(uint16_t blockNumber, char *block) {
    uint32_t index = superBlock.blockSize * superBlock.dataStartBlock + superBlock.blockSize * blockNumber;

    disk.seekg(index, std::ios::beg);
    disk.write((char *) block, superBlock.blockSize);
}

void FileSystem::readSuperBlock() {
    disk.seekg(0, std::ios::beg);
    disk.read((char *) &superBlock, sizeof(SuperBlock));
}

void FileSystem::readInodeBitmap() {
    disk.seekg(superBlock.inodeBitmapStartBlock * superBlock.blockSize, std::ios::beg);
    disk.read((char *) inodeBitmap, superBlock.inodeCount);
}

void FileSystem::readBlockBitmap() {
    disk.seekg(superBlock.blockBitmapStartBlock * superBlock.blockSize, std::ios::beg);
    disk.read((char *) blockBitmap, superBlock.blockCount);
}

void FileSystem::writeSuperBlock() {
    disk.seekg(0, std::ios::beg);
    disk.write((char *) &superBlock, sizeof(SuperBlock));
}

void FileSystem::writeInodeBitmap() {
    disk.seekg(superBlock.inodeBitmapStartBlock * superBlock.blockSize, std::ios::beg);
    disk.write((char *) inodeBitmap, superBlock.inodeCount);
}

void FileSystem::writeBlockBitmap() {
    disk.seekg(superBlock.blockBitmapStartBlock * superBlock.blockSize, std::ios::beg);
    disk.write((char *) blockBitmap, superBlock.blockCount);
}
