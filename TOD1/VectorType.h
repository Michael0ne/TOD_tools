#pragma once
#include "DataType.h"

class VectorType : public DataType
{
public:
	VectorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
	virtual ~VectorType();

	virtual void*	stub3(void*) const override;	//	@864770
	virtual String& stub6(String&, void*, int) const override;	//	@8657D0
	virtual int		stub7(char*, void*) const override;	//	@8647A0
	virtual void	stub13(int, void*, int, int, int, void* const) const override;	//	@864850
	virtual void	stub14(int*, int, void*, int, int, int) const override;	//	@8648B0
	virtual bool	stub16(void*, void*) const override;	//	@864A30
	virtual void	stub17(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override;	//	@864B20
	virtual void	stub18(int operationId, void* params) const override;	//	@864E60
	virtual bool	stub20(void*) const override;	//	@864930

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