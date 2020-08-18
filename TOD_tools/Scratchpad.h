#pragma once

#include "stdafx.h"

#include "MemoryAllocators.h"

#define SCRATCHPAD_CLASS_SIZE 1076

class Scratchpad
{
private:
	char	_pad[1076];
public:
	Scratchpad()
	{
		MESSAGE_CLASS_CREATED(Scratchpad);

		memset(&_pad, 0, sizeof(_pad));
		patch(0xA3DE4C, this, 4);
	}
	~Scratchpad()
	{
		MESSAGE_CLASS_DESTROYED(Scratchpad);
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}
};

extern Scratchpad* g_Scratchpad;

static_assert(sizeof(Scratchpad) == SCRATCHPAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Scratchpad));