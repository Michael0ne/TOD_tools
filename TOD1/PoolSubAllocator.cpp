#include "PoolSubAllocator.h"

PoolSubAllocator::PoolSubAllocator(int objectsize, int alignment)
{
    MESSAGE_CLASS_CREATED(PoolSubAllocator);

    ObjectSize = objectsize;
    SizeAlignment = ALIGN_8BYTESUP(alignment);
    ObjectSpace = nullptr;
}

PoolSubAllocator::~PoolSubAllocator()
{
    MESSAGE_CLASS_DESTROYED(PoolSubAllocator);
}

void* PoolSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    stub9();

    if (!ObjectSpace)
        return nullptr;

    void* spacePtr = ObjectSpace;

    ObjectSpace = (uint8_t*)*ObjectSpace;
    ObjectsCount++;
    OccupiedSpaceSize = ObjectSizeAligned + OccupiedSpaceSize;

    stub9();

    return spacePtr;
}

void* PoolSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    return alignment == SizeAlignment ? Allocate_A(size, fileName, fileLineNumber) : nullptr;
}

void PoolSubAllocator::Free(void* ptr)
{
    stub9();

    OccupiedSpaceSize -= ObjectSizeAligned;
    ObjectsCount--;
    *(uint8_t**)ptr = (uint8_t*)ObjectSpace;
    ObjectSpace = (uint8_t*)ptr;

    if (ProfilerEnabled)
        memset((uint32_t*)ptr + 1, 0xAB, (ObjectSizeAligned - 4) / sizeof(uint32_t));

    stub9();
}

void PoolSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

void* PoolSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    stub9();

    if (!newsize)
    {
        Free(oldptr);
        return nullptr;
    }

    return oldptr;
}

int PoolSubAllocator::stub8(int* unk)
{
    return ObjectSizeAligned;
}

void PoolSubAllocator::stub9()
{
    return;
}

void PoolSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    uint8_t* bufferBeginPtrAligned = (uint8_t*)(~(SizeAlignment - 1) & (uint32_t)((uint8_t*)bufferptr + (SizeAlignment - 1)));
    const uint8_t* bufferEndPtr = ((uint8_t*)bufferptr + AllocatedSpaceSize);

    ObjectSpace = bufferBeginPtrAligned;
    BlocksCount = NULL;
    ObjectSizeAligned = ~(SizeAlignment - 1) & ObjectSize + SizeAlignment - 1;

    if (!bufferBeginPtrAligned)
    {
        stub9();
        return;
    }

    do
    {
        if (&bufferBeginPtrAligned[ObjectSizeAligned]  >= bufferEndPtr)
            break;

        BlocksCount++;

        //  NOTE: set the first element of each buffer 'entry' to point to the next 'entry'.
        const uint32_t nextAlignedAddress = (uint32_t)&bufferBeginPtrAligned[ObjectSizeAligned * 2];
        const bool nextAddressOutOfBounds = (nextAlignedAddress - (uint32_t)bufferEndPtr) < 0;
        *(uint32_t*)bufferBeginPtrAligned = (uint32_t)&bufferBeginPtrAligned[ObjectSizeAligned] & (nextAddressOutOfBounds ? 0 : -1);

        //  NOTE: if profiler is enabled, then fill the rest of the buffer with dummy data.
        if (ProfilerEnabled)
            memset(bufferBeginPtrAligned + 4, 0xAB, ObjectSizeAligned - 4);

            bufferBeginPtrAligned = (uint8_t*)(*(uint32_t*)bufferBeginPtrAligned);
    } while (bufferBeginPtrAligned);

    stub9();
}

const int PoolSubAllocator::GetTotalAllocations() const
{
    return OccupiedSpaceSize;
}

const int PoolSubAllocator::GetAllocatedElementsTotal() const
{
    return ObjectsCount;
}

const char* const PoolSubAllocator::GetAllocatorName() const
{
    return "PoolSubAllocator";
}

const int PoolSubAllocator::stub19() const
{
    return ObjectsCount;
}