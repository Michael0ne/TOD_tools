#pragma once
#include "AllocatorBase.h"

class FirstFitSubAllocator : public Allocator
{
    struct HeaderData
    {
        HeaderData*     Next;
        uint32_t        Flags;
    };

protected:
    uint32_t        PointerDataSize;  //  NOTE: how much space is reserved to store information about pointer.
    uint32_t       *SpacePtr;   //  NOTE: this points to a big allocated chunk of space initially. The mapping is: [0] -> pointer to the next 'chunk 'block' inside this chunk, [1] -> is it being used or not, ... -> the next space up to the address pointed at by [0] is occupied for actual object.
    uint32_t        SpaceOccupied;
    uint32_t        FreeRegions;
    uint32_t        UsedRegions;
    uint32_t       *CurrentDataBlockPtr;

private:
    bool            UpdateUsedBlocks(uint32_t* ptr);  //  @4798D0
    uint32_t*       GetHeaderDataPtr(void* ptr) const;  //  @479950 //  NOTE: unused.
    uint32_t*       AlignPointer(const uint32_t* ptr, const uint32_t size) const;    //  @479960 //  NOTE: unused.
    void            FreeBlockIfSmall(uint32_t* ptr, const uint32_t sizeAvailable, const int32_t sizeRequested);    //  @479C00

    static const size_t AlignmentDefault = 8;

public:
    FirstFitSubAllocator(); // @4797F0
    ~FirstFitSubAllocator()
    {
        MESSAGE_CLASS_DESTROYED(FirstFitSubAllocator);
    }

    virtual void*       Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @479830
    virtual void*       AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @479C40
    virtual void        Free(void* ptr) override; // @479A10
    virtual void        FreeAligned(void* ptr) override; // @479850
    virtual void*       Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @479DA0
    virtual uint32_t    GetDataSize(uint32_t* ptr) override; // @479AA0
    virtual void        stub9() override;
    virtual void        SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @479990
    virtual const int   GetFreeMemory() const override; // @479860
    virtual const char* const GetAllocatorName() const override; // @479980
    virtual const int   GetUsedBlocksTotal() const override; // @479880
    virtual const int   GetFreeBlocksTotal() const override; // @479890
    virtual const int   GetBiggestUsedMemoryBlock() const override; // @479BB0
    virtual const int   GetAvailableMemory() const override; // @479B70
    virtual void        Dump() const override; // @479AE0
};

ASSERT_CLASS_SIZE(FirstFitSubAllocator, 60);