#pragma once
#include "DataType.h"

class IntegerType : public DataType
{
public:
    IntegerType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~IntegerType();

    virtual void* ReturnNew(void*) const override; // @87AA10
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @8643B0
    virtual int  MakeFromString(const char* const input, char* const outdata) const override; // @863F30
    virtual void CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, uint8_t* const outResult) const override; // @489450
    virtual bool NotEqualTo(const void* const arg1, const void* const arg2) const override; // @863F50
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char*) const override; // @863F70
    virtual void PerformOperation(int operationId, void* params) const override; // @864200
    virtual bool IsValidValueForType(const void*) const override; // @489440

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