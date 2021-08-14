#pragma once
#include "DataType.h"

class IntegerType : public DataType
{
public:
	IntegerType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
	virtual ~IntegerType();

	virtual void* stub3(void*) const override;	//	@87AA10
	virtual String& stub6(String&, void*, int) const override;	//	@8643B0
	virtual int		stub7(char*, void*) const override;	//	@863F30
	virtual void	stub13(int, void*, int, int, int, void* const) const override;	//	@489450
	virtual bool	stub16(void*, void*) const override;	//	@863F50
	virtual void	stub17(const char* const operation, int* outopid, DataType** outoprestype, char*) const override;	//	@863F70
	virtual void	stub18(int operationId, void* params) const override;	//	@864200
	virtual bool	stub20(void*) const override;	//	@489440

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