#pragma once
#include "AllocatorBase.h"

class SequentialSubAllocator : public Allocator
{
protected:
    uint32_t    field_24;
    uint32_t    AllocationsTotal;
    uint8_t    *ValidRegionEnd;
    uint8_t    *RegionBegin;
    uint8_t    *RegionBegin_1;
    uint32_t    field_38;
    uint32_t    ObjectSize;

    uint32_t    GetNextAlignedAddress(const uint32_t alignment); // @47A4A0

public:
    SequentialSubAllocator(); // @47A420

    virtual void*   Allocate(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A480
    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A460
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @47A5B0
    virtual void    Free(void* ptr) override; // @47A610
    virtual void    FreeAligned(void* ptr) override; // @47A070
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47A640
    virtual uint32_t    GetDataSize(uint32_t* ptr) override; // @47A4E0
    virtual void    stub9() override;
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47A500
    virtual const int   GetFreeMemory() const override; // @47A540
    virtual const int   GetAllocatedElementsTotal() const override; // @47A550
    virtual const char* const GetAllocatorName() const override; // @47A5A0
    virtual const int   GetUsedBlocksTotal() const override; // @47A560
    virtual int     GetMemoryReserved() override; // @47A570
    virtual void    stub36() override;
};

ASSERT_CLASS_SIZE(SequentialSubAllocator, 64);