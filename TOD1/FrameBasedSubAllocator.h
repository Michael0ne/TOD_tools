#pragma once
#include "SequentialSubAllocator.h"

class FrameBasedSubAllocator : public SequentialSubAllocator
{
    struct AllocatorRegionInfo
    {
        int                     field_0;
        char                   *m_RegionPtr;
        AllocatorRegionInfo    *m_PreviousRegionPtr;
    };
protected:
    AllocatorRegionInfo        *m_AllocSpaceInfo;

public:
    FrameBasedSubAllocator();	//	@479EE0

    virtual void*               Allocate_A(size_t size, int filler, int unk) override;	//	@47A050
    virtual void*               AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;	//	@479F60
    virtual void                Free(void* ptr) override;	//	@479FF0
    virtual void                FreeAligned(void* ptr) override;	//	@47A070
    virtual void*               Realloc(void* oldptr, size_t newsize, int filler, int unk) override;	//	@47A040
    virtual int                 stub8(int* unk) override;	//	@47A4E0
    virtual void                stub9() override;	//	@47A080
    virtual void                SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override;	//	@479F20
    virtual const char* const   GetAllocatorName() const override;	//	@47A170
    virtual const int           stub21() const override;	//	@47A150
    virtual const int           GetAvailableMemory() const override;	//	@47A160
    virtual int                 stub35() override;	//	@47A090
    virtual void                stub36() override;	//	@479F10

    void                        _47A120();	//	@47A120
    void                        _47A0E0();  //  @47A0E0
};

ASSERT_CLASS_SIZE(FrameBasedSubAllocator, 68);