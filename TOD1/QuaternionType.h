#pragma once
#include "DataType.h"

class QuaternionType : public DataType
{
public:
    QuaternionType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~QuaternionType();

    virtual void* ReturnNew(void*) const override; // @865890
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @867810
    virtual int  MakeFromString(const char* const input, char* const outdata) const override; // @8658F0
    virtual void CallGetterFunction(Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const override; // @8659E0
    virtual void CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const override; // @865A40
    virtual bool NotEqualTo(void*, void*) const override; // @8678E0
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override; // @865C10
    virtual void PerformOperation(int operationId, void* params) const override; // @866090
    virtual bool IsValidValueForType(void*) const override; // @865AC0

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