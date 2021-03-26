#pragma once
#include "BaseType.h"

class StringType : public BaseType
{
public:
	StringType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
	virtual ~StringType();

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