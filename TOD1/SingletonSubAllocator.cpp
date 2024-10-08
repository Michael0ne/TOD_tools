#include "SingletonSubAllocator.h"

SingletonSubAllocator::SingletonSubAllocator()
{
    MESSAGE_CLASS_CREATED(SingletonSubAllocator);

    Created = false;
    DataPtr = nullptr;
}

void* SingletonSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    if (size > AllocatedSpaceSize)
        return nullptr;

    DataPtr = AllocatedSpacePtr;
    Created = true;

    return DataPtr;
}

void* SingletonSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    void* alignedspaceptr = (void*)(~(alignment - 1) & (int)((char*)AllocatedSpacePtr + alignment - 1));

    DataPtr = alignedspaceptr;
    Created = true;

    if (size > (size_t)((char*)AllocatedSpacePtr + AllocatedSpaceSize - (char*)alignedspaceptr))
        return nullptr;
    else
        return alignedspaceptr;
}

void SingletonSubAllocator::Free(void* ptr)
{
    Created = false;
    DataPtr = nullptr;
}

void SingletonSubAllocator::FreeAligned(void* ptr)
{
    Created = false;
    DataPtr = nullptr;
}

void* SingletonSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (newsize <= (size_t)(AllocatedSpaceSize + (char*)AllocatedSpacePtr - (char*)DataPtr))
        return oldptr;

    if (newsize > AllocatedSpaceSize)
        return nullptr;

    DataPtr = AllocatedSpacePtr;

    return DataPtr;
}

uint32_t SingletonSubAllocator::GetDataSize(uint32_t* ptr)
{
    return AllocatedSpaceSize;
}

void SingletonSubAllocator::stub9()
{
    return;
}

const int SingletonSubAllocator::GetFreeMemory() const
{
    return Created ? AllocatedSpaceSize : NULL;
}

const char* const SingletonSubAllocator::GetAllocatorName() const
{
    return "SingletonSubAllocator";
}

const int SingletonSubAllocator::GetUsedBlocksTotal() const
{
    return Created;
}

const int SingletonSubAllocator::GetFreeBlocksTotal() const
{
    return !Created;
}

const int SingletonSubAllocator::GetBiggestUsedMemoryBlock() const
{
    return Created ? NULL : AllocatedSpaceSize;
}

const int SingletonSubAllocator::GetAvailableMemory() const
{
    return Created ? NULL : AllocatedSpaceSize;
}