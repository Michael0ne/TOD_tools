#pragma once
#include "DataType.h"

class TruthType : public DataType
{
public:
    TruthType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~TruthType();

    virtual void* ReturnNew(void*) const override; // @87AA10
    virtual String& PrintFormattedValue(String&, void*, int) const override; // @864740
    virtual int  MakeFromString(const char* const input, char* const outdata) const override; // @864440
    virtual void CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, uint8_t* const outResult) const override; // @8644B0
    virtual void CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const override; // @864500
    virtual void ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char*) const override; // @864560
    virtual void PerformOperation(int operationId, void* params) const override; // @864660

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