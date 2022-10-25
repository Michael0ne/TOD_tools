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
    uint32_t*       _478E80(uint32_t* ptr);   //  @478E80

    static unsigned int   MinimumSize; // @A3B550

public:
    BestFitAllocator(); // @478740

    virtual void*       Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @478800
    virtual void*       AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @478820

    virtual void        Free(void* ptr) override; // @479210
    virtual void        FreeAligned(void* ptr) override; // @4788E0

    virtual void*       Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @4795C0
    virtual uint32_t    stub8(uint32_t* ptr) override; // @478AA0
    virtual void        stub9();
    virtual void        CallMethodAtOffset20() override; // @478A90
    virtual void        SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @478DF0
    virtual const int   GetFreeMemory() const override; //  @478AD0
    virtual const int   GetAllocatedElementsTotal() const override; // @478DC0
    virtual const int   GetAllocationsMadeTotal() const override; // @478B70
    virtual const int   stub20() const override; // @478B80
    virtual const int   stub21() const override; // @478AE0
    virtual const int   GetAvailableMemory() const override; // @478B90
    virtual void        Dump() const override; // @478BE0
    virtual int         stub24(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @478C90
    virtual int         stub25(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber, int alignment) override; // @479350
    virtual int*        GetMemoryContents() const override; // @478CF0
    virtual const int   stub27() const override; // @478D00
    virtual int         stub28(int*) const override; // @478D10
    virtual const int   stub29(int*) const override; // @478D30
    virtual char        OwnsPointer(int*) const override; // @478D50
    virtual char        stub31(uint32_t* ptr, const uint32_t size, const uint32_t alignment) const override; // @478D60
    virtual int         stub32(int) const override; // @478DD0
    virtual int         stub33(int) const override; // @478DE0
    virtual char        TryExpandBy(int*, int) override; // @479490
    virtual int         GetMemoryReserved() override; // @478B60
};

ASSERT_CLASS_SIZE(BestFitAllocator, 292);