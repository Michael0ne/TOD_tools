#include "StackBasedSubAllocator.h"

StackBasedSubAllocator::StackBasedSubAllocator()
{
    MESSAGE_CLASS_CREATED(StackBasedSubAllocator);

    Stack = nullptr;
    StackCopy = nullptr;
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
    uint8_t* alignedspace = (uint8_t*)((~(alignment - 1) & (uint32_t)(Stack->DataPtr + alignment - 1)));
    if ((size_t)(StackDataEndPtr - alignedspace) < size)
        return nullptr;

    StackElement* newelement = (StackElement*)(alignedspace + (uint32_t)Stack - Stack->DataPtr);
    newelement->Previous = Stack->Previous;
    newelement->Next = Stack->Next;
    newelement->DataPtr = Stack->DataPtr;
    Stack = newelement;

    if (Stack->Previous)
        Stack->Previous->Next = Stack;
    else
        StackCopy = Stack;

    newelement = (StackElement*)(Stack->DataPtr + ALIGN_4BYTESUP(size));
    newelement->Next = nullptr;
    newelement->Previous = Stack;
    Stack->Next = newelement;
    Stack = newelement;

    ElementsInStack++;

    return newelement->Previous->DataPtr;
}

void StackBasedSubAllocator::Free(void* ptr)
{
    if (!ptr)
        return;

    --ElementsInStack;
    Stack->Next = nullptr;
    Stack = Stack->Previous;
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
    Stack->Previous->Next = nullptr;
    Stack = Stack->Previous;

    StackElement* newelementptr = (StackElement*)(Stack->DataPtr + ALIGN_4BYTESUP(newsize));
    newelementptr->Next = nullptr;
    newelementptr->Previous = Stack;
    Stack->Next = newelementptr;
    Stack = newelementptr;
    ElementsInStack++;

    return newelementptr->Previous->DataPtr;
}

uint32_t StackBasedSubAllocator::stub8(uint32_t* ptr)
{
    return (ptr - DataSize + 4) - (ptr - DataSize) - 12;
}

void StackBasedSubAllocator::stub9()
{
    if (!StackCopy)
        return;

    // TODO: maybe this function should have return type?
    StackElement* prevelement = StackCopy;
    for (prevelement = prevelement->Previous; prevelement; prevelement = prevelement->Previous);
}

void StackBasedSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    Stack = (StackElement*)AllocatedSpacePtr;
    StackCopy = (StackElement*)AllocatedSpacePtr;
    StackDataEndPtr = (uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize;
    *(char*)AllocatedSpacePtr = NULL;
    Stack->Next = nullptr;
}

const int StackBasedSubAllocator::GetFreeMemory() const
{
    return (int)Stack - (int)StackCopy->DataPtr;
}

const char* const StackBasedSubAllocator::GetAllocatorName() const
{
    return "StackBasedSubAllocator";
}

const int StackBasedSubAllocator::GetAllocationsMadeTotal() const
{
    return ElementsInStack;
}

const int StackBasedSubAllocator::stub20() const
{
    return 1;
}

const int StackBasedSubAllocator::stub21() const
{
    return GetAvailableMemory();
}

const int StackBasedSubAllocator::GetAvailableMemory() const
{
    return (uint32_t)StackDataEndPtr - ALIGN_16BYTESUP((uint32_t)Stack);
}