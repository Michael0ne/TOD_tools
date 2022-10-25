#pragma once
#include "AllocatorBase.h"

class SystemSubAllocator : public Allocator
{
protected:
    uint32_t        AllocationsTotal;

public:
    SystemSubAllocator(); // @47AB90

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47AC00
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @47AC30
    virtual void    Free(void* ptr) override; // @47AC90
    virtual void    FreeAligned(void* ptr) override; // @47AC70
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47ACB0
    virtual uint32_t    stub8(uint32_t* ptr) override; // @47AD60
    virtual void    stub9() override; // @47ADE0
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47ADF0
    virtual const int   GetAllocatedSpaceSize() const override; // @47ADA0
    virtual const int   GetFreeMemory() const override; // @47AD30
    virtual const char* const GetAllocatorName() const override; // @47AE10
    virtual const int   GetAllocationsMadeTotal() const override; // @47AD70
    virtual const int   stub20() const override; // @47AD70
    virtual const int   stub21() const override; // @47AD90
    virtual const int   GetAvailableMemory() const override; // @47ADC0
    virtual int     GetMemoryReserved() override; // @47AE00
};

ASSERT_CLASS_SIZE(SystemSubAllocator, 40);