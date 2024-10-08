#pragma once
#include "AllocatorBase.h"

class BestFitAllocator : public Allocator
{
    struct BlockInfo
    {
        uint32_t   *DataPtr;
        uint32_t    field_4;
        uint32_t    field_8;
    };

protected:
    BlockInfo   field_24[20];
    uint32_t   *DataPtr;
    uint32_t    BytesAvailable;
    uint32_t    BlocksFree;
    uint32_t    BlocksUsed;

private:
    uint32_t        FindSuitableBlock(const uint32_t size) const;   //  @479780
    uint32_t*       _479290(uint32_t* ptr, const uint32_t a2, const uint32_t requestedSize);   //  @479290
    void            _478E80(uint32_t* ptr);   //  @478E80
    void            _478F70(uint32_t* ptr); //  @478F70
    void            _478910(const uint32_t blockIndex); //  @478910
    void            _4790C0(uint32_t* ptr, uint32_t* ptr1); //  @4790C0
    void            _479180(uint32_t* ptr); //  @479180

    static uint32_t MinimumSize; // @A3B550

public:
    BestFitAllocator(); // @478740

    virtual void*       Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @478800
    virtual void*       AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @478820

    virtual void        Free(void* ptr) override; // @479210
    virtual void        FreeAligned(void* ptr) override; // @4788E0

    virtual void*       Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @4795C0
    virtual uint32_t    GetDataSize(uint32_t* ptr) override; // @478AA0
    virtual void        stub9();
    virtual void        CallMethodAtOffset20() override; // @478A90
    virtual void        SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @478DF0
    virtual const int   GetFreeMemory() const override; //  @478AD0
    virtual const int   GetAllocatedElementsTotal() const override; // @478DC0
    virtual const int   GetUsedBlocksTotal() const override; // @478B70
#ifdef INCLUDE_FIXES
    virtual const char* const GetAllocatorName() const override; //  NOTE: not in original code.
#endif
    virtual const int   GetFreeBlocksTotal() const override; // @478B80
    virtual const int   GetBiggestUsedMemoryBlock() const override; // @478AE0
    virtual const int   GetAvailableMemory() const override; // @478B90
    virtual void        Dump() const override; // @478BE0
    virtual int         PlacementNew(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @478C90
    virtual int         PlacementNewAligned(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber, int alignment) override; // @479350
    virtual int*        GetMemoryContents() const override; // @478CF0
    virtual const int   stub27() const override; // @478D00
    virtual uint32_t*   GetNextDataPtr(int*) const override; // @478D10
    virtual const int   stub29(int*) const override; // @478D30
    virtual char        OwnsPointer(int*) const override; // @478D50
    virtual char        CanExpandData(uint32_t* ptr, const uint32_t size, const uint32_t alignment) const override; // @478D60
    virtual uint32_t*   GetObjectDataPtr(void* ptr) const override; // @478DD0
    virtual uint32_t*   GetHeaderDataPtr(void* ptr) const override; // @478DE0
    virtual char        TryExpandBy(uint8_t* ptr, uint32_t size) override; // @479490
    virtual int         GetMemoryReserved() override; // @478B60
};

ASSERT_CLASS_SIZE(BestFitAllocator, 292);