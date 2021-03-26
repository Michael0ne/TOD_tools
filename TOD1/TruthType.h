#pragma once
#include "BaseType.h"

class TruthType : public BaseType
{
public:
	TruthType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
	virtual ~TruthType();

	virtual void*	stub3(void*) const override;	//	@87AA10
	virtual String& stub6(String&, void*, int) const override;	//	@864740
	virtual int		stub7(char*, void*) const override;	//	@864440
	virtual void	stub13(int, void*, int, int, int, void* const) const override;	//	@8644B0
	virtual void	stub14(int*, int, void*, int, int, int) const override;	//	@864500
	virtual void	stub17(const char* const operation, int* outopid, BaseType** outoprestype, char*) const override;	//	@864560
	virtual void	stub18(int operationId, void* params) const override;	//	@864660

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