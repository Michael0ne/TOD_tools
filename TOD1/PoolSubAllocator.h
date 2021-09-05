#pragma once
#include "AllocatorBase.h"

class PoolSubAllocator : public Allocator
{
protected:
 int       m_TotalOccupiedSpace;
 int       m_ObjectsInPool;
 void*      m_ObjectSpace; // NOTE: pointer to linked list, struct { void* m_NextSpacePtr; 
 int       m_ObjectSizeAligned;
 int       m_ObjectSize;
 int       field_38;
 int       field_3C;

public:
 PoolSubAllocator(int objectsize, int alignment); // @47A1B0

 virtual void*    Allocate_A(size_t size, int filler, int unk) override; // @47A1F0
 virtual void*    AllocateAligned(size_t size, size_t alignment, int filler, int unk) override; // @47A230
 virtual void    Free(void* ptr) override; // @47A260
 virtual void    FreeAligned(void* ptr) override; // @47A2C0
 virtual void*    Realloc(void* oldptr, size_t newsize, int filler, int unk) override; // @47A2D0
 virtual int     stub8(int* unk) override; // @47A300
 virtual void    stub9() override;
 virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47A320
 virtual const int   GetTotalAllocations() const override; // @47A3D0
 virtual const int   GetAllocatedElementsTotal() const override; // @47A3E0
 virtual const char* const GetAllocatorName() const override; // @47A410
 virtual const int   stub19() const override; // @47A3F0
};

ASSERT_CLASS_SIZE(PoolSubAllocator, 64);