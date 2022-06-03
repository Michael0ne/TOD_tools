#pragma once
#include "AllocatorBase.h"

class FirstFitSubAllocator : public Allocator
{
    //  NOTE: single linked list.
    struct FirstFitElement
    {
        FirstFitElement* m_Next;
        void* m_SpacePtr;
    };

protected:
    int       field_24;
    FirstFitElement* m_Space;
    int       m_SpaceOccupied;
    int       m_FreeRegions;
    int       m_UsedRegions;
    FirstFitElement* m_Space_1;

private:
    bool        _4798D0(FirstFitElement* element);  //  @4798D0

    static const size_t AlignmentDefault = 8;

public:
    FirstFitSubAllocator(); // @4797F0

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @479830
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @479C40
    virtual void    Free(void* ptr) override; // @479A10
    virtual void    FreeAligned(void* ptr) override; // @479850
    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @479DA0
    virtual int     stub8(int* unk) override; // @479AA0
    virtual void    stub9() override;
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @479990
    virtual const int   GetTotalAllocations() const override; // @479860
    virtual const char* const GetAllocatorName() const override; // @479980
    virtual const int   stub19() const override; // @479880
    virtual const int   stub20() const override; // @479890
    virtual const int   stub21() const override; // @479BB0
    virtual const int   GetAvailableMemory() const override; // @479B70
    virtual void    Dump() const override; // @479AE0
};

ASSERT_CLASS_SIZE(FirstFitSubAllocator, 60);