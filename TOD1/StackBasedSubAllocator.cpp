#include "StackBasedSubAllocator.h"

StackBasedSubAllocator::StackBasedSubAllocator()
{
    MESSAGE_CLASS_CREATED(StackBasedSubAllocator);

    StackCurrentPtr = nullptr;
    StackBeginPtr = nullptr;
    StackDataEndPtr = nullptr;
    ElementsInStack = NULL;
    DataSize = 8;
}

void* StackBasedSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

void* StackBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    uint8_t* alignedspace = (uint8_t*)((~(alignment - 1) & (uint32_t)(StackCurrentPtr->DataPtr + alignment - 1)));
    if ((size_t)(StackDataEndPtr - alignedspace) < size)
        return nullptr;

    StackElement* newelement = (StackElement*)(alignedspace + (uint32_t)StackCurrentPtr - StackCurrentPtr->DataPtr);
    newelement->Previous = StackCurrentPtr->Previous;
    newelement->Next = StackCurrentPtr->Next;
    newelement->DataPtr = StackCurrentPtr->DataPtr;
    StackCurrentPtr = newelement;

    if (StackCurrentPtr->Previous)
        StackCurrentPtr->Previous->Next = StackCurrentPtr;
    else
        StackBeginPtr = StackCurrentPtr;

    newelement = (StackElement*)(StackCurrentPtr->DataPtr + ALIGN_4BYTESUP(size));
    newelement->Next = nullptr;
    newelement->Previous = StackCurrentPtr;
    StackCurrentPtr->Next = newelement;
    StackCurrentPtr = newelement;

    ElementsInStack++;

    return newelement->Previous->DataPtr;
}

void StackBasedSubAllocator::Free(void* ptr)
{
    if (!ptr)
        return;

    --ElementsInStack;
    StackCurrentPtr->Next = nullptr;
    StackCurrentPtr = StackCurrentPtr->Previous;
}

void StackBasedSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

void* StackBasedSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (!oldptr)
        return Allocate_A(newsize, fileName, fileLineNumber);

    if (!newsize)
    {
        Free(oldptr);
        return nullptr;
    }

    --ElementsInStack;
    StackCurrentPtr->Previous->Next = nullptr;
    StackCurrentPtr = StackCurrentPtr->Previous;

    StackElement* newelementptr = (StackElement*)(StackCurrentPtr->DataPtr + ALIGN_4BYTESUP(newsize));
    newelementptr->Next = nullptr;
    newelementptr->Previous = StackCurrentPtr;
    StackCurrentPtr->Next = newelementptr;
    StackCurrentPtr = newelementptr;
    ElementsInStack++;

    return newelementptr->Previous->DataPtr;
}

uint32_t StackBasedSubAllocator::GetDataSize(uint32_t* ptr)
{
    return (ptr - DataSize + 4) - (ptr - DataSize) - 12;
}

void StackBasedSubAllocator::stub9()
{
    if (!StackBeginPtr)
        return;

    // TODO: maybe this function should have return type?
    StackElement* prevelement = StackBeginPtr;
    for (prevelement = prevelement->Previous; prevelement; prevelement = prevelement->Previous);
}

void StackBasedSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    StackCurrentPtr = (StackElement*)AllocatedSpacePtr;
    StackBeginPtr = (StackElement*)AllocatedSpacePtr;
    StackDataEndPtr = (uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize;
    *((uint32_t*)AllocatedSpacePtr) = NULL;
    StackCurrentPtr->Next = nullptr;
}

const int StackBasedSubAllocator::GetFreeMemory() const
{
    return (uint32_t)StackCurrentPtr - (uint32_t)StackBeginPtr + DataSize;
}

const char* const StackBasedSubAllocator::GetAllocatorName() const
{
    return "StackBasedSubAllocator";
}

const int StackBasedSubAllocator::GetUsedBlocksTotal() const
{
    return ElementsInStack;
}

const int StackBasedSubAllocator::GetFreeBlocksTotal() const
{
    return 1;
}

const int StackBasedSubAllocator::GetBiggestUsedMemoryBlock() const
{
    return GetAvailableMemory();
}

const int StackBasedSubAllocator::GetAvailableMemory() const
{
    return (uint32_t)StackDataEndPtr - ALIGN_16BYTESUP((uint32_t)StackCurrentPtr);
}