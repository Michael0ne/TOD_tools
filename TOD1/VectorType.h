#pragma once
#include "DataType.h"

class VectorType : public DataType
{
public:
    VectorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~VectorType();

    virtual void* ReturnNew(void*) const override; // @864770
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @8657D0
    virtual int  MakeFromString(const char* const input, char* const outdata) const override; // @8647A0
    virtual void CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const override; // @864850
    virtual void CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const override; // @8648B0
    virtual bool NotEqualTo(const void* const arg1, const void* const arg2) const override; // @864A30
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override; // @864B20
    virtual void PerformOperation(int operationId, void* params) const override; // @864E60
    virtual bool IsValidValueForType(const void*) const override; // @864930

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