#pragma once
#include "DataType.h"

class DictType : public DataType
{
protected:
    DataType* m_ElementsType;

public:
    DictType(DataType* elementsType); // @867B50
    virtual ~DictType();

    virtual int  GetSize(int*, int*); // @867C00
    virtual void* ReturnNew(void*) const; // @869800
    virtual void Delete(char*); // @868630
    virtual void Clone(int*, int*); // @868A20
    virtual String& PrintFormattedValue(String&, void*, int) const; // @867D50
    virtual int  MakeFromString(char*, void*) const; // @868700
    virtual int  CopyNoAllocate(char*, char*); // @868100
    virtual int  CopyAndAllocate(char*, char*); // @868830
    virtual bool NotEqualTo(void*, void*) const; // @868240
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const; // @8682D0
    virtual void PerformOperation(int operationId, void* params) const; // @868C30
    virtual char IsReferenced(int, int); // @8685A0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete (void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }
};