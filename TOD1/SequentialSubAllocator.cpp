#include "SequentialSubAllocator.h"

uint32_t SequentialSubAllocator::GetNextAlignedAddress(const uint32_t alignment)
{
    return (~(alignment - 1) & (uint32_t)&RegionBegin[ObjectSize - 1 + alignment]) - ObjectSize;
}

SequentialSubAllocator::SequentialSubAllocator()
{
    MESSAGE_CLASS_CREATED(SequentialSubAllocator);

    RegionBegin = nullptr;
    RegionBegin_1 = nullptr;
    field_24 = NULL;
    AllocationsTotal = NULL;
    ValidRegionEnd = nullptr;
    field_38 = NULL;
    ObjectSize = NULL;
}

void* SequentialSubAllocator::Allocate(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 4, fileName, fileLineNumber);
}

void* SequentialSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

void* SequentialSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    const uint32_t freeAlignedAddress = GetNextAlignedAddress(alignment);
    const size_t alignedObjectSize = ALIGN_4BYTESUP(size);

    //  NOTE: no more free space, return nothing.
    if (freeAlignedAddress + size + ObjectSize > (uint32_t)ValidRegionEnd)
        return nullptr;

    if (RegionBegin == RegionBegin_1)
        RegionBegin_1 = (uint8_t*)freeAlignedAddress;

    field_38 = freeAlignedAddress;
    RegionBegin = (uint8_t*)freeAlignedAddress;

    ++AllocationsTotal;

    return (uint8_t*)(freeAlignedAddress + ObjectSize);
}

void SequentialSubAllocator::Free(void* ptr)
{
    AllocationsTotal--;

    if ((uint32_t)ptr == field_38)
        field_38 = NULL;

    //  NOTE: if no more allocations, reset everything.
    if (!AllocationsTotal)
        stub35();
}

void SequentialSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

void* SequentialSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (!oldptr)
        return Allocate_A(newsize, fileName, fileLineNumber);

    if (newsize > NULL &&
        (uint32_t)oldptr + newsize <= (uint32_t)ValidRegionEnd)
    {
        RegionBegin = (uint8_t*)oldptr + newsize;
        return oldptr;
    }
    else
    {
        Free(oldptr);
        return nullptr;
    }
}

int SequentialSubAllocator::stub8(int* unk)
{
    return NULL;
}

void SequentialSubAllocator::stub9()
{
    return;
}

void SequentialSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);
    ValidRegionEnd = (uint8_t*)((uint32_t)bufferptr + size);
    RegionBegin_1 = (uint8_t*)bufferptr;
    RegionBegin = (uint8_t*)bufferptr;

    stub36();
}

const int SequentialSubAllocator::GetTotalAllocations() const
{
    return RegionBegin - RegionBegin_1;
}

const int SequentialSubAllocator::GetAllocatedElementsTotal() const
{
    return AllocationsTotal;
}

const char* const SequentialSubAllocator::GetAllocatorName() const
{
    return "SequentialSubAllocator";
}

const int SequentialSubAllocator::stub19() const
{
    return AllocationsTotal;
}

int SequentialSubAllocator::stub35()
{
    RegionBegin_1 = (uint8_t*)AllocatedSpacePtr;
    RegionBegin = (uint8_t*)AllocatedSpacePtr;
    field_38 = NULL;
    stub36();
    field_24 = NULL;
    AllocationsTotal = NULL;

    // FIXME: original code doesn't return anything, but this virtual function SHOULD return something.
    return NULL;
}

void SequentialSubAllocator::stub36()
{
    return;
}