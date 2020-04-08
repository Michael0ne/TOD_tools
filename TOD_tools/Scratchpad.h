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
		memset(&_pad, 0, sizeof(_pad));

		debug("Scratchpad created at %X\n", this);
	}

	~Scratchpad()
	{
		debug("Scratchpad destroyed!\n");
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	Init();	//	@8C1650
};

extern Scratchpad* g_Scratchpad;

static_assert(sizeof(Scratchpad) == SCRATCHPAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Scratchpad"));