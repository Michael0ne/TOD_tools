#include "FrameBasedSubAllocator.h"
#include "LogDump.h"

FrameBasedSubAllocator::SpaceInfo* FrameBasedSubAllocator::_479FB0(uint8_t* ptr)
{
    SpaceInfo* dataPtr = ObjectSpace;
    if (ptr > dataPtr->DataPtr)
        return dataPtr;

    if (!dataPtr)
        return nullptr;

    while (true)
    {
        if (ptr < dataPtr->DataPtr)
        {
            if (dataPtr->PreviousElement && ptr >= dataPtr->PreviousElement->DataPtr)
                break;
        }

        dataPtr = dataPtr->PreviousElement;
        if (!dataPtr)
            return nullptr;
    }

    return dataPtr->PreviousElement;
}

FrameBasedSubAllocator::FrameBasedSubAllocator()
{
    MESSAGE_CLASS_CREATED(FrameBasedSubAllocator);
}

void* FrameBasedSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

void* FrameBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    if (ObjectSpace->DataPtr == RegionBegin)
        ObjectSpace->DataPtr += GetNextAlignedAddress(alignment) - (uint32_t)RegionBegin;

    void* result = SequentialSubAllocator::AllocateAligned(size, alignment, fileName, fileLineNumber);
    if (result)
        ObjectSpace->field_0++;

    return result;
}

void FrameBasedSubAllocator::Free(void* ptr)
{
    if ((uint8_t*)ptr >= ObjectSpace->DataPtr)
    {
        ObjectSpace->field_0--;
        SequentialSubAllocator::Free(ptr);

        return;
    }

    if (!ObjectSpace)
    {
        ObjectSpace->field_0--;
        SequentialSubAllocator::Free(ptr);

        return;
    }

    SpaceInfo* objectSpace = ObjectSpace;
    while (true)
    {
        if ((uint8_t*)ptr < objectSpace->DataPtr &&
            objectSpace->PreviousElement &&
            (uint8_t*)ptr >= objectSpace->PreviousElement->DataPtr)
                break;

        objectSpace = objectSpace->PreviousElement;
        if (!objectSpace)
        {
            objectSpace = nullptr;
            objectSpace->field_0--;
            SequentialSubAllocator::Free(ptr);

            return;
        }
    }

    objectSpace->PreviousElement->field_0--;
    SequentialSubAllocator::Free(ptr);
}

void FrameBasedSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

void* FrameBasedSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    return SequentialSubAllocator::Realloc(oldptr, newsize, fileName, fileLineNumber);
}

uint32_t FrameBasedSubAllocator::stub8(uint32_t* ptr)
{
    return NULL;
}

void FrameBasedSubAllocator::stub9()
{
    SequentialSubAllocator::stub9();
}

void FrameBasedSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    SequentialSubAllocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    ObjectSpace->field_0 = NULL;
    ObjectSpace->PreviousElement = nullptr;
    ObjectSpace->DataPtr = RegionBegin;
}

const char* const FrameBasedSubAllocator::GetAllocatorName() const
{
    return "FrameBasedSubAllocator";
}

const int FrameBasedSubAllocator::stub21() const
{
    return ValidRegionEnd - RegionBegin;
}

const int FrameBasedSubAllocator::GetAvailableMemory() const
{
    return stub21();
}

int FrameBasedSubAllocator::GetMemoryReserved()
{
    SpaceInfo* objectSpace = ObjectSpace;
    if (objectSpace)
    {
        while (true)
        {
            if (!objectSpace->PreviousElement)
                break;

            Free(objectSpace);
            objectSpace = objectSpace->PreviousElement;
        }
    }

    SequentialSubAllocator::GetMemoryReserved();

    ObjectSpace->field_0 = NULL;
    ObjectSpace->PreviousElement = nullptr;
    ObjectSpace->DataPtr = RegionBegin;

    return (int)RegionBegin;
}

void FrameBasedSubAllocator::stub36()
{
    ObjectSpace = (SpaceInfo*)RegionBegin;

    RegionBegin += 48;
    RegionBegin_1 = RegionBegin;
}

void FrameBasedSubAllocator::MakeNew()
{
    SpaceInfo* newElement = (SpaceInfo*)Allocate_A(sizeof(SpaceInfo), __FILE__, __LINE__);
    newElement->field_0 = NULL;
    newElement->DataPtr = RegionBegin;
    newElement->PreviousElement = ObjectSpace;

    ObjectSpace = newElement;
}

void FrameBasedSubAllocator::RemoveLast()
{
    if (ObjectSpace->field_0)
        LogDump::LogA("Log output only enabled in debug builds.\n");

    SpaceInfo* currentObject = ObjectSpace;
    ObjectSpace = ObjectSpace->PreviousElement;
    Free(currentObject);

    if (RegionBegin != RegionBegin_1)
        RegionBegin = currentObject->DataPtr;
}