#pragma once
#include "AllocatorBase.h"

class BestFitAllocator : public Allocator
{
    struct BestFitAllocInfo
    {
        int* field_0;
        int field_4;
        int field_8;
        int field_C;
        int field_10;
        int field_14;
        int field_18;
        int field_1C;
        int field_20;
        int field_24;
        int field_28;
        int field_2C;
        int field_30;
        int field_34;
        int field_38;
    };

protected:
    BestFitAllocInfo   field_24[4];
    int      *m_Contents;
    int       field_118;
    int       field_11C;
    int       field_120;

private:
    const int     _479780(const int* const) const; // @479780

    static unsigned int   MinimumSize; // @A3B550

public:
    BestFitAllocator(); // @478740

    virtual void*   Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber) override; // @478800
    virtual void*   AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber) override; // @478820

    virtual void    Free(void* ptr) override; // @479210
    virtual void    FreeAligned(void* ptr) override; // @4788E0

    virtual void*   Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber) override; // @4795C0
    virtual int     stub8(int* unk) override; // @478AA0
    virtual void    stub9();
    virtual void    CallMethodAtOffset20() override; // @478A90
    virtual void    SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override; // @478DF0
    virtual const int   GetTotalAllocations() const override;
    virtual const int   GetAllocatedElementsTotal() const override; // @478DC0
    virtual const int   stub19() const override; // @478B70
    virtual const int   stub20() const override; // @478B80
    virtual const int   stub21() const override; // @478AE0
    virtual const int   GetAvailableMemory() const override; // @478B90
    virtual void    Dump() const override; // @478BE0
    virtual int     stub24(int, int, const char* const fileName, const unsigned int fileLineNumber) override; // @478C90
    virtual int     stub25(int, int, const char* const fileName, const unsigned int fileLineNumber, int alignment) override; // @479350
    virtual int* GetMemoryContents() const override; // @478CF0
    virtual const int   stub27() const override; // @478D00
    virtual int     stub28(int*) const override; // @478D10
    virtual const int   stub29(int*) const override; // @478D30
    virtual char    stub30(int*) const override; // @478D50
    virtual char    stub31(int*, int, int) const override; // @478D60
    virtual int     stub32(int) const override; // @478DD0
    virtual int     stub33(int) const override; // @478DE0
    virtual char    stub34(int*, int) override; // @479490
    virtual int     stub35() override; // @478B60
};

ASSERT_CLASS_SIZE(BestFitAllocator, 292);