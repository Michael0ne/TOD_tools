#pragma once
#include "DataType.h"

class StringType : public DataType
{
public:
    StringType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize); // @879EB0
    virtual ~StringType();

    virtual int  GetSize(int*, int*); // @87A9E0
    virtual void* ReturnNew(void*) const; // @87AA10
    virtual void Delete(char*); // @87AA20
    virtual void Clone(int*, int*); // @5145F0

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