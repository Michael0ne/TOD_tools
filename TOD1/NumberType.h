#pragma once
#include "DataType.h"

class NumberType : public DataType
{
public:
    NumberType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~NumberType();

    virtual void* stub3(void*) const override; // @863890
    virtual String& stub6(String&, void*, int) const override; // @863E90
    virtual int  stub7(char*, void*) const override; // @862AA0
    virtual void stub13(int, void*, int, int, int, void* const) const override; // @8638C0
    virtual void stub14(int*, int, void*, int, int, int) const override; // @4894A0
    virtual bool stub16(void*, void*) const override; // @863980
    virtual void stub17(const char* const operation, int* outopid, DataType** outoprestype, char*) const override; // @8639B0
    virtual void stub18(int operationId, void* params) const override; // @863C10
    virtual bool stub20(void*) const override; // @863920

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