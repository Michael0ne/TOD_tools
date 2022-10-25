#pragma once
#include "AllocatorBase.h"

class SingletonSubAllocator : public Allocator
{
protected:
    bool        Created;
    void       *DataPtr;

public:
    SingletonSubAllocator(); // @47A690

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A6B0
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @47A6D0
    virtual void    Free(void* ptr) override; // @47A720
    virtual void    FreeAligned(void* ptr) override; // @47A710
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47A730
    virtual uint32_t    stub8(uint32_t* ptr) override; // @47A770
    virtual void    stub9() override;
    virtual const int   GetFreeMemory() const override; // @47A790
    virtual const char* const GetAllocatorName() const override; // @47A7F0
    virtual const int   GetAllocationsMadeTotal() const override; // @47A7A0
    virtual const int   stub20() const override; // @47A7B0
    virtual const int   stub21() const override; // @47A7C0
    virtual const int   GetAvailableMemory() const override; // @47A7D0
};

ASSERT_CLASS_SIZE(SingletonSubAllocator, 44);