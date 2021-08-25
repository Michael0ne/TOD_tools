#pragma once
#include "DataType.h"

class StringType : public DataType
{
public:
	StringType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);	//	@879EB0
	virtual ~StringType();

	virtual int		stub2(int*, int*);	//	@87A9E0
	virtual void*	stub3(void*) const;	//	@87AA10
	virtual void	stub4(char*);	//	@87AA20
	virtual void	stub5(int*, int*);	//	@5145F0

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