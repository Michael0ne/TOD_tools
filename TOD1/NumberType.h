#pragma once
#include "DataType.h"

class NumberType : public DataType
{
public:
    NumberType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~NumberType();

    virtual void* ReturnNew(void*) const override; // @863890
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @863E90
    virtual int  StrToType(char*, void*) const override; // @862AA0
    virtual void stub13(int, int(__thiscall* procptr)(void*, void*), int, int, int, void* const outResult) const override;  //  @8638C0
    virtual void stub14(int*, int, void*, int, int, int) const override; // @4894A0
    virtual bool NotEqualTo(void*, void*) const override; // @863980
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override; // @8639B0
    virtual void PerformOperation(int operationId, void* params) const override; // @863C10
    virtual bool IsValidValueForType(void*) const override; // @863920

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