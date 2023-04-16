#ifndef __MYOS__VIRTUALMEMORY_H
#define __MYOS__VIRTUALMEMORY_H

#include <common/types.h>

using namespace myos::common;

namespace myos
{
    struct PageTable
    {
        uint32_t pageFrameNumber;
        uint64_t counter;
        bool present = false;
        bool referenced = false;
    };

    class MMU
    {
    public:
        uint32_t *virtualMemory;

        uint32_t pageSize;
        PageTable pageTable[1024];

        uint32_t memorySize;
        uint32_t memoryStartAddress;
        uint32_t memoryEndAddress;
        uint32_t diskSize;
        uint32_t diskStartAddress;
        uint32_t diskEndAddress;

        uint32_t pageCount;
        uint32_t pageTableSize;

        MMU(uint32_t startAddress, uint32_t size);
        ~MMU();

        uint32_t getPageIndex();
        uint32_t getValue(uint32_t virtualAddress);
        void setValue(uint32_t virtualAddress, uint32_t getValue);

        // Statistics
        uint32_t pageMissCount = 0;
        uint32_t pageHitCount = 0;
        uint32_t pageLoadCount = 0;
        uint32_t pageWriteCount = 0;

    };


}

#endif
