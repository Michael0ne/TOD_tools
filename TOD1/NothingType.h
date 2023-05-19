#pragma once
#include "DataType.h"

class NothingType : public DataType
{
public:
    NothingType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~NothingType();

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