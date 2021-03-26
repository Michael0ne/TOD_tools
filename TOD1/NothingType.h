#pragma once
#include "BaseType.h"

class NothingType : public BaseType
{
public:
	NothingType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
	virtual ~NothingType();

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