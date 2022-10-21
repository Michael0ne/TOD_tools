#include "FirstFitSubAllocator.h"
#include "LogDump.h"

bool FirstFitSubAllocator::_4798D0(uint32_t* ptr)
{
    bool result = false;

    do
    {
        if (ptr >= (uint32_t*)*ptr)
            break;

        result = true;

        if (SpacePtr_1 == (uint32_t*)*ptr)
            SpacePtr_1 = ptr;

        uint32_t* nextDataBlock = (uint32_t*)*ptr;
        *ptr = ptr[0];

        if (ProfilerEnabled)
            memset(nextDataBlock, 0xAB, PointerDataSize);

        FreeRegions--;
    } while (!*(uint32_t*)(*ptr + 4));

    if (SpacePtr == (uint32_t*)*ptr && !SpacePtr[1] && ptr != SpacePtr)
        _4798D0(SpacePtr);

    return result;
}

void FirstFitSubAllocator::FreeBlockIfSmall(uint32_t* ptr, const uint32_t sizeAvailable, const int32_t sizeRequested)
{
    const uint32_t sizeRequestedAligned = ALIGN_8BYTESUP(sizeRequested + PointerDataSize);
    if (sizeAvailable > sizeRequestedAligned)
    {
        uint32_t* nextBlockPtr = (ptr + sizeRequestedAligned);
        nextBlockPtr[1] = NULL;
        *nextBlockPtr = *ptr;

        *ptr = (int32_t)ptr + sizeRequestedAligned;

        FreeRegions++;
    }
}

FirstFitSubAllocator::FirstFitSubAllocator()
{
    MESSAGE_CLASS_CREATED(FirstFitSubAllocator);

    PointerDataSize = 8;
    SpacePtr = NULL;
    SpaceOccupied = NULL;
    FreeRegions = NULL;
    UsedRegions = NULL;
    SpacePtr_1 = NULL;
}

void* FirstFitSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

void* FirstFitSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    if (ProfilerEnabled)
        stub9();

    if (alignment < AlignmentDefault)
        alignment = AlignmentDefault;

    uint32_t* dataBlockStartPtr = SpacePtr_1;
    uint32_t dataBlockPtrModulo = 0;
    uint32_t nextDataBlockSize;

    while (true)
    {
        uint32_t* dataBlockPtr = SpacePtr_1;
        if (!dataBlockPtr[1])
        {
            if (_4798D0(SpacePtr_1))
            {
                uint32_t* nextDatablockPtr = SpacePtr_1;
                if (*nextDatablockPtr <= (uint32_t)nextDatablockPtr)
                {
                    if (dataBlockStartPtr > nextDatablockPtr)
                        dataBlockStartPtr = SpacePtr;
                }
                else
                    if (dataBlockStartPtr > nextDatablockPtr && dataBlockStartPtr < (uint32_t*)*nextDatablockPtr)
                        dataBlockStartPtr = SpacePtr_1;
            }

            dataBlockPtr = SpacePtr_1;
            uint32_t* nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
            if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
                nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);
            nextDataBlockSize = nextDataBlockPtr - PointerDataSize - dataBlockPtr;
            dataBlockPtrModulo = (uint32_t)(dataBlockPtr + 2) % alignment;
            uint32_t closestAvailableSize = size;
            if (dataBlockPtrModulo)
                closestAvailableSize = alignment + size - dataBlockPtrModulo;

            if (nextDataBlockSize >= closestAvailableSize)
                break;
        }

        SpacePtr_1 = (uint32_t*)*dataBlockPtr;
        //  NOTE: if the next free block pointer is same as what we started with, then there's no fit.
        if (SpacePtr_1 == dataBlockStartPtr)
            return nullptr;
    }

    if (dataBlockPtrModulo)
    {
        uint32_t* currentDataBlockPtr = SpacePtr_1;
        const uint32_t closestAlignedAvailableSize = ALIGN_8BYTESUP(dataBlockPtrModulo + alignment - PointerDataSize);
        uint32_t* blockPtr = nullptr;

        if (nextDataBlockSize > closestAlignedAvailableSize)
        {
            blockPtr = (uint32_t*)((uint8_t*)currentDataBlockPtr + closestAlignedAvailableSize);
            blockPtr[1] = 0;
            blockPtr[0] = *currentDataBlockPtr;

            *currentDataBlockPtr = (uint32_t)currentDataBlockPtr + closestAlignedAvailableSize;

            FreeRegions++;
        }

        SpacePtr_1 = blockPtr;

        uint32_t* nextDataBlockPtr = (uint32_t*)*blockPtr;
        if (*blockPtr <= (uint32_t)blockPtr)
            nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

        nextDataBlockSize = nextDataBlockPtr - blockPtr - PointerDataSize;
    }

    uint32_t* currentDataBlockPtr = SpacePtr_1;
    const uint32_t alignedSize = ALIGN_8BYTESUP(size + PointerDataSize);

    if (nextDataBlockSize > alignedSize)
    {
        uint32_t* blockPtr = (uint32_t*)((uint8_t*)currentDataBlockPtr + alignedSize);
        blockPtr[0] = currentDataBlockPtr[0];
        blockPtr[1] = 0;

        *currentDataBlockPtr = (uint32_t)blockPtr;
        currentDataBlockPtr = SpacePtr_1;

        nextDataBlockSize = blockPtr - currentDataBlockPtr - PointerDataSize;

        FreeRegions++;
    }

    currentDataBlockPtr[1] = 1;

    FreeRegions--;
    UsedRegions++;
    SpaceOccupied += nextDataBlockSize + PointerDataSize;

    return SpacePtr_1 + 2;
}

void FirstFitSubAllocator::Free(void* ptr)
{
    if (ProfilerEnabled)
        stub9();

    if (!ptr)
        return;

    uint32_t* blockStartPtr = ((uint32_t*)ptr - PointerDataSize);
    uint32_t* blockPointer = (uint32_t*)*blockStartPtr;

    blockStartPtr[1] = NULL;

    if (blockPointer <= blockStartPtr)
        blockPointer = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    SpaceOccupied += blockStartPtr - blockPointer;

    FreeRegions++;
    UsedRegions--;

    if (ProfilerEnabled)
        memset(ptr, 0xAB, blockPointer - PointerDataSize - blockStartPtr);

    _4798D0(blockStartPtr);
}

void FirstFitSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

void* FirstFitSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (!oldptr)
        return Allocate_A(newsize, fileName, fileLineNumber);

    if (!newsize)
    {
        Free(oldptr);

        return nullptr;
    }

    uint32_t* dataBlockPtr = (uint32_t*)((uint8_t*)oldptr - PointerDataSize);
    uint32_t* nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
    if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
        nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    const uint32_t objectSize = (uint32_t)((uint8_t*)nextDataBlockPtr - (uint8_t*)oldptr);
    uint32_t fitSize;
    while (true)
    {
        nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
        if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
            nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

        fitSize = (uint32_t)nextDataBlockPtr - PointerDataSize - (uint32_t)dataBlockPtr;
        if (newsize <= fitSize)
            break;

        if (!_4798D0(dataBlockPtr))
        {
            void* newDataPtr = Allocate_A(newsize, fileName, fileLineNumber);
            void* newDataPtrCopy = newDataPtr;

            if (newDataPtr)
            {
                nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
                if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
                    nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

                memcpy(newDataPtr, oldptr, nextDataBlockPtr - PointerDataSize - dataBlockPtr);
                Free(oldptr);

                return newDataPtrCopy;
            }

            return newDataPtr;  //  NOTE: really?
        }
    }

    if (ProfilerEnabled)
        memset((uint8_t*)oldptr + newsize, 0xAB, fitSize - newsize);

    FreeBlockIfSmall(dataBlockPtr, fitSize, newsize);

    nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
    if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
        nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    SpaceOccupied += nextDataBlockPtr - PointerDataSize - objectSize - dataBlockPtr;

    return oldptr;
}

int FirstFitSubAllocator::stub8(int* unk)
{
    if (!unk)
        return NULL;

    int32_t* pointerInfo = (unk - PointerDataSize);
    int32_t actualPointer = *pointerInfo;
    if (*pointerInfo <= (int32_t)pointerInfo)
        actualPointer = (int32_t)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    if (ProfilerEnabled)
        stub9();

    return actualPointer - (int32_t)unk;
}

void FirstFitSubAllocator::stub9()
{
    return;
}

void FirstFitSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    uint32_t* blockStartPtr = (uint32_t*)(ALIGN_8BYTESUP((uint32_t)AllocatedSpacePtr + PointerDataSize) - PointerDataSize);
    *blockStartPtr = NULL;
    SpacePtr = blockStartPtr;
    *SpacePtr = (uint32_t)SpacePtr;
    SpacePtr_1 = SpacePtr;

    SpaceOccupied = 0;
    FreeRegions = 1;
    UsedRegions = 0;

    if (ProfilerEnabled)
        memset(SpacePtr + 2, 0xAB, (uint32_t)AllocatedSpacePtr + AllocatedSpaceSize - (uint32_t)(SpacePtr + 2));
}

const int FirstFitSubAllocator::GetTotalAllocations() const
{
    return SpaceOccupied;
}

const char* const FirstFitSubAllocator::GetAllocatorName() const
{
    return "FirstFitSubAllocator";
}

const int FirstFitSubAllocator::stub19() const
{
    return UsedRegions;
}

const int FirstFitSubAllocator::stub20() const
{
    return FreeRegions;
}

const int FirstFitSubAllocator::stub21() const
{
    uint32_t biggestFreeBlock = 0;
    uint32_t* dataBlockPtr = SpacePtr;

    do
    {
        if (!dataBlockPtr[1])
        {
            uint32_t* nextDataBlock = (uint32_t*)*dataBlockPtr;
            if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
                nextDataBlock = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

            const uint32_t currentBlockSize = nextDataBlock - PointerDataSize - dataBlockPtr;
            if (currentBlockSize > biggestFreeBlock)
            {
                nextDataBlock = (uint32_t*)*dataBlockPtr;
                if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
                    nextDataBlock = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

                biggestFreeBlock = nextDataBlock - PointerDataSize - dataBlockPtr;
            }
        }

        dataBlockPtr = (uint32_t*)*dataBlockPtr;
    } while (SpacePtr != dataBlockPtr);

    return biggestFreeBlock;
}

const int FirstFitSubAllocator::GetAvailableMemory() const
{
    int32_t memoryAvailable = 0;

    uint32_t* dataBlockPtr = SpacePtr;
    do
    {
        if (!dataBlockPtr[1])
        {
            uint32_t* nextDataBlockPtr = (uint32_t*)*dataBlockPtr;
            if (*dataBlockPtr <= (uint32_t)dataBlockPtr)
                nextDataBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

            memoryAvailable = (int32_t)(nextDataBlockPtr - PointerDataSize - (uint32_t)dataBlockPtr);
        }

        dataBlockPtr = (uint32_t*)*dataBlockPtr;
    } while (SpacePtr != dataBlockPtr);

    return memoryAvailable;
}

void FirstFitSubAllocator::Dump() const
{
    LogDump::LogA("FirstFitAllocator contents:\n");
    LogDump::LogA("-----------------------------\n");

    uint32_t* spaceStartPtr = SpacePtr;
    for (uint32_t i = 0; i < FreeRegions + UsedRegions; ++i)
    {
        uint32_t* nextBlockPtr = (uint32_t*)*spaceStartPtr;
        if (*spaceStartPtr <= (uint32_t)spaceStartPtr)
            nextBlockPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

        LogDump::LogA("%d\t: %x\t : %s\t : %d\n", i, spaceStartPtr, spaceStartPtr[1] ? "used" : "free", nextBlockPtr - PointerDataSize - (uint32_t)spaceStartPtr);

        spaceStartPtr = (uint32_t*)spaceStartPtr[0];
    }

    LogDump::LogA("-----------------------------\n");
}