#pragma once
#include "DataType.h"

class NumberType : public DataType
{
public:
    NumberType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~NumberType();

    virtual void* ReturnNew(void*) const override; // @863890
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @863E90
    virtual int  MakeFromString(const char* const input, char* const outdata) const override; // @862AA0
    virtual void CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, uint8_t* const outResult) const override;  //  @8638C0
    virtual void CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const override; // @4894A0
    virtual bool NotEqualTo(const void* const arg1, const void* const arg2) const override; // @863980
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override; // @8639B0
    virtual void PerformOperation(int operationId, void* params) const override; // @863C10
    virtual bool IsValidValueForType(const void*) const override; // @863920

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