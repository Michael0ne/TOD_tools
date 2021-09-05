#pragma once
#include "AllocatorBase.h"

class SequentialSubAllocator : public Allocator
{
protected:
 int       field_24;
 int       m_AllocationsTotal;
 char*      m_ValidRegionEnd;
 char*      m_RegionBegin;
 char*      m_RegionBegin_1;
 int       field_38;
 int       field_3C;

 int       _47A4A0(int); // @47A4A0

public:
 SequentialSubAllocator(); // @47A420

 virtual void*    Allocate(size_t size, int filler, int unk) override; // @47A480
 virtual void*    Allocate_A(size_t size, int filler, int unk) override; // @47A460
 virtual void*    AllocateAligned(size_t size, size_t alignment, int filler, int unk) override; // @47A5B0
 virtual void    Free(void* ptr) override; // @47A610
 virtual void    FreeAligned(void* ptr) override; // @47A070
 virtual void*    Realloc(void* oldptr, size_t newsize, int filler, int unk) override; // @47A640
 virtual int     stub8(int* unk) override; // @47A4E0
 virtual void    stub9() override;
 virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @47A500
 virtual const int   GetTotalAllocations() const override; // @47A540
 virtual const int   GetAllocatedElementsTotal() const override; // @47A550
 virtual const char* const GetAllocatorName() const override; // @47A5A0
 virtual const int   stub19() const override; // @47A560
 virtual int     stub35() override; // @47A570
 virtual void    stub36() override;
};

ASSERT_CLASS_SIZE(SequentialSubAllocator, 64);