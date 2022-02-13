#pragma once
#include "DataType.h"

class TruthType : public DataType
{
public:
    TruthType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~TruthType();

    virtual void* ReturnNew(void*) const override; // @87AA10
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @864740
    virtual int  StrToType(char*, void*) const override; // @864440
    virtual void stub13(int, int(__thiscall* procptr)(void*, void*), int, int, int, void* const outResult) const override; // @8644B0
    virtual void stub14(int*, int, void*, int, int, int) const override; // @864500
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char*) const override; // @864560
    virtual void PerformOperation(int operationId, void* params) const override; // @864660

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