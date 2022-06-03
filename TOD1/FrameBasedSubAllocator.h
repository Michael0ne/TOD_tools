#pragma once
#include "SequentialSubAllocator.h"

class FrameBasedSubAllocator : public SequentialSubAllocator
{
    struct SpaceInfo
    {
        uint32_t    field_0;    //  NOTE: index?
        uint8_t    *DataPtr;
        SpaceInfo  *PreviousElement;
    };

protected:
    SpaceInfo      *ObjectSpace;

public:
    FrameBasedSubAllocator(); // @479EE0

    virtual void*               Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @47A050
    virtual void*               AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @479F60
    virtual void                Free(void* ptr) override; // @479FF0
    virtual void                FreeAligned(void* ptr) override; // @47A070
    virtual void*               Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @47A040
    virtual int                 stub8(int* unk) override; // @47A4E0
    virtual void                stub9() override; // @47A080
    virtual void                SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @479F20
    virtual const char* const   GetAllocatorName() const override; // @47A170
    virtual const int           stub21() const override; // @47A150
    virtual const int           GetAvailableMemory() const override; // @47A160
    virtual int                 stub35() override; // @47A090
    virtual void                stub36() override; // @479F10

    void                        MakeNew(); // @47A120
    void                        RemoveLast();  //  @47A0E0
};

ASSERT_CLASS_SIZE(FrameBasedSubAllocator, 68);