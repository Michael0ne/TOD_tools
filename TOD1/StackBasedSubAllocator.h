#pragma once
#include "AllocatorBase.h"

class StackBasedSubAllocator : public Allocator
{
    struct StackElement
    {
        StackElement   *Previous;
        StackElement   *Next;
        uint8_t        *DataPtr;
    };

protected:
    StackElement   *Stack;
    StackElement   *StackCopy;
    uint8_t        *StackDataEndPtr;
    uint32_t        ElementsInStack;
    uint32_t        DataSize;

public:
    StackBasedSubAllocator(); // @47A820

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A9D0
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @47A930
    virtual void    Free(void* ptr) override; // @47A9F0
    virtual void    FreeAligned(void* ptr) override; // @47AA10
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47AA20
    virtual uint32_t    stub8(uint32_t* ptr) override; // @47AAA0
    virtual void    stub9() override; // @47AAC0
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47A890
    virtual const int   GetFreeMemory() const override; // @47AAD0
    virtual const char* const GetAllocatorName() const override; // @47AB20
    virtual const int   GetAllocationsMadeTotal() const override; // @47AAE0
    virtual const int   stub20() const override; // @47AAF0
    virtual const int   stub21() const override; // @47AB10
    virtual const int   GetAvailableMemory() const override; // @47AB00
};

ASSERT_CLASS_SIZE(StackBasedSubAllocator, 56);