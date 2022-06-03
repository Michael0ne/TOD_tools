#pragma once
#include "AllocatorBase.h"

class PoolSubAllocator : public Allocator
{
protected:
    uint32_t    OccupiedSpaceSize;  //  NOTE: total size of the allocated buffer in bytes.
    uint32_t    ObjectsCount;
    uint8_t    *ObjectSpace;    //  NOTE: linked list essentially.
    uint32_t    SizeAlignment;  //  NOTE: align each object size by this value.
    uint32_t    ObjectSize; //  NOTE: size of each object in the pool.
    uint32_t    BlocksCount;    //  NOTE: how many objects are in the pool.
    uint32_t    ObjectSizeAligned;  //  NOTE: aligned object size.

public:
    PoolSubAllocator(int objectsize, int alignment); // @47A1B0
    virtual ~PoolSubAllocator();

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A1F0
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @47A230
    virtual void    Free(void* ptr) override; // @47A260
    virtual void    FreeAligned(void* ptr) override; // @47A2C0
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47A2D0
    virtual int     stub8(int* unk) override; // @47A300
    virtual void    stub9() override;
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47A320
    virtual const int   GetTotalAllocations() const override; // @47A3D0
    virtual const int   GetAllocatedElementsTotal() const override; // @47A3E0
    virtual const char* const GetAllocatorName() const override; // @47A410
    virtual const int   stub19() const override; // @47A3F0
};

ASSERT_CLASS_SIZE(PoolSubAllocator, 64);