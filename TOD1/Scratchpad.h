#pragma once
#include "MemoryManager.h"

#define SCRATCHPAD_CLASS_SIZE 1076

class Scratchpad
{
private:
	char	_pad[1076] = {};
public:
	Scratchpad();
	~Scratchpad();

	void* operator new (size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

extern Scratchpad* g_Scratchpad;

ASSERT_CLASS_SIZE(Scratchpad, 1076);