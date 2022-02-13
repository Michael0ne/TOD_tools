#pragma once
#include "DataType.h"

class ColorType : public DataType
{
public:
    ColorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~ColorType();

    virtual String& PrintFormattedValue(String&, void*, int) const override; // @867A50
    virtual void stub13(int, int(__thiscall* procptr)(void*, void*), int, int, int, void* const outResult) const override;
    virtual void stub14(int*, int, void*, int, int, int) const override;

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete (void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }
};