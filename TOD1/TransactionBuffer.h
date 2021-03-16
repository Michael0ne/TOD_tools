#pragma once
#include "MemoryAllocators.h"

class TransactionBuffer
{
	friend class Scene;
protected:
	void*			m_Buffer;
	int				m_Chunks;
	int				m_Size;
	int				field_C;
	std::list<int>	m_List_1;
	void*			field_1C;
	class Entity*	field_20;

public:
	TransactionBuffer(unsigned int bufferSize);	//	@8AADA0
	~TransactionBuffer();	//	@875210

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, false);
		ptr = nullptr;
	}
};

ASSERT_CLASS_SIZE(TransactionBuffer, 36);