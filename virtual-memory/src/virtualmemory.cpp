#include <virtualmemory.h>

using namespace myos;
using namespace myos::common;

MMU::MMU(uint32_t startAddress, uint32_t size)
{
    virtualMemory = (uint32_t *)startAddress;
    
    this->pageSize = sizeof(int) * 4;
    
}

uint32_t MMU::getPageIndex()
{
    return 0;
}

uint32_t MMU::getValue(uint32_t virtualAddress)
{
    uint32_t pageIndex;
    uint32_t index;
    uint32_t pageNumber = virtualAddress / pageSize;
    uint32_t offset = virtualAddress % pageSize;

    if (pageTable[pageNumber].present)
    {
        pageHitCount++;
    }
    else
    {   
        pageMissCount++;
        pageLoadCount++;
        pageWriteCount;

        pageIndex = getPageIndex();
        pageTable[pageIndex].present = false;
        pageTable[pageNumber].present = true;
    }

    index = pageTable[pageNumber].pageFrameNumber + offset;
    return virtualMemory[index];
}

void MMU::setValue(uint32_t virtualAddress, uint32_t value)
{
    uint32_t pageIndex;
    uint32_t index;
    uint32_t pageNumber = virtualAddress / pageSize;
    uint32_t offset = virtualAddress % pageSize;

    if (pageTable[pageNumber].present)
    {
        pageHitCount++;
    }
    else
    {   
        pageMissCount++;
        pageLoadCount++;
        pageWriteCount;
        
        pageIndex = getPageIndex();
        pageTable[pageIndex].present = false;
        pageTable[pageNumber].present = true;
    }

    index = pageTable[pageNumber].pageFrameNumber + offset;
    virtualMemory[index] = value;
}