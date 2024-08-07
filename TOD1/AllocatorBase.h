#pragma once
#include "stdafx.h"

enum AllocatorIndex
{
    DEFAULT = 0,
    MAIN_ASSETS = 1,
    MISSION_ASSETS = 2,
    CUTSCENE_OR_REWIND = 3,
    PLAYER_DATA = 4,
    TEMP = 5,
    RENDERLIST = 6,
    SCRATCHPAD = 7,
    COLLISION_CACHE_ENTRIES = 8,
    DEFRAGMENTING = 9,

    TOTAL = 10
};

class DefragmentatorBase;

class Allocator
{
    friend class MemoryManager;

    struct DefaultLowLevelAllocator
    {
        void*     (*_malloc)(size_t size);
        void     (*_free)(void* ptr);

        DefaultLowLevelAllocator();
    };

protected:
    void*                   AllocatedSpacePtr;
    size_t                  AllocatedSpaceSize;
    bool                    ProfilerEnabled;
    DefaultLowLevelAllocator&   LowLevelAllocators;
    const char*             AllocatorName;

public:
    AllocatorIndex          AllocatorIndex;
    DefragmentatorBase     *Defragmentator;
    char                    field_20;
    char                    field_21;

private:
    DefaultLowLevelAllocator&  GetSystemAllocatorsTable() const; // @4775C0

public:
    Allocator(); // @47AB30
    virtual      ~Allocator(); // @478410

    virtual void*       Allocate(size_t size, const char* const fileName, const unsigned int fileLineNumber); // @478340
    virtual void*       Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) = 0;
    virtual void*       AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) = 0;

    virtual void        Free(void* ptr) = 0;
    virtual void        FreeAligned(void* ptr) = 0;

    virtual void*       Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) = 0;
    virtual uint32_t    GetDataSize(uint32_t* ptr) = 0;
    virtual void        stub9() = 0;    //  NOTE: something like 'ProfileMemory'.
    virtual void        CallMethodAtOffset20(); // @478350
    virtual void        SetField21(char); // @478360
    virtual void        SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size); // @47AB60
    virtual void*       GetAllocatedSpacePtr() const; // @478370
    virtual const int   GetAllocatedSpaceSize() const; // @419C40
    virtual const int   GetFreeMemory() const;
    virtual const int   GetAllocatedElementsTotal() const;
    virtual const char* const GetAllocatorName() const; // @47AB80
    virtual void        SetProfilerEnabled(bool); // @478380
    virtual const int   GetUsedBlocksTotal() const;
    virtual const int   GetFreeBlocksTotal() const;
    virtual const int   GetBiggestUsedMemoryBlock() const;
    virtual const int   GetAvailableMemory() const;
    virtual void        Dump() const;
    virtual int         PlacementNew(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber);
    virtual int         PlacementNewAligned(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber, int alignment);
    virtual int*        GetMemoryContents() const;
    virtual const int   stub27() const;
    virtual uint32_t*   GetNextDataPtr(int*) const;
    virtual const int   stub29(int*) const;
    virtual char        OwnsPointer(int*) const;
    virtual char        CanExpandData(uint32_t* ptr, const uint32_t size, const uint32_t alignment) const;
    virtual uint32_t*   GetObjectDataPtr(void* ptr) const;
    virtual uint32_t*   GetHeaderDataPtr(void* ptr) const;
    virtual char        TryExpandBy(uint8_t* ptr, uint32_t size);
    virtual int         GetMemoryReserved();
    virtual void        stub36();
};

ASSERT_CLASS_SIZE(Allocator, 36);