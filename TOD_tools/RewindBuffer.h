#pragma once

#include "stdafx.h"
#include "MemoryAllocators.h"

class Entity;

#define REWINDBUFFER_CLASS_SIZE 36

class RewindBuffer_List
{
public:
	int		field_0;
	void*	field_4;
	int		field_8;

public:
	void	_8751D0();	//	@8751D0
};

class RewindBuffer
{
public:
	void*	m_pBuffer;
	int		m_nChunks;
	int		m_nSize;
	int		field_C;
	RewindBuffer_List m_UnknownList;
	void*	field_1C;
	Entity*	field_20;


public:
	RewindBuffer(const unsigned int bufferSize);	//	@8AADA0
	~RewindBuffer()
	{
		MESSAGE_CLASS_DESTROYED(RewindBuffer);
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, false);
	}
};

static_assert(sizeof(RewindBuffer) == REWINDBUFFER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(RewindBuffer));