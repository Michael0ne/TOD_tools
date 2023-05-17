#pragma once
#include "DataType.h"

class ColorType : public DataType
{
public:
    ColorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
    virtual ~ColorType();

    virtual String& PrintFormattedValue(String&, void*, int) const override; // @867A50
    virtual void CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, uint8_t* const outResult) const override;
    virtual void CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const override;

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