#pragma once
#include "BaseType.h"

class DictType : public BaseType
{
protected:
	BaseType*	m_ElementsType;

public:
	DictType(BaseType* elementsType);	//	@867B50
	virtual ~DictType();

	virtual int		stub2(int*, int*);	//	@867C00
	virtual void*	stub3(void*) const;	//	@869800
	virtual void	stub4(char*);	//	@868630
	virtual void	stub5(int*, int*);	//	@868A20
	virtual String& stub6(String&, void*, int) const;	//	@867D50
	virtual int		stub7(char*, void*) const;	//	@868700
	virtual int		stub9(char*, char*);	//	@868100
	virtual int		stub10(char*, char*);	//	@868830
	virtual bool	stub16(void*, void*) const;	//	@868240
	virtual void	stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const;	//	@8682D0
	virtual void	stub18(int operationId, void* params) const;	//	@868C30
	virtual char	stub19(int, int);	//	@8685A0

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