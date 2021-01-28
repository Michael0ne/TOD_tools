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

class TransactionBuffer
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
	TransactionBuffer(const unsigned int bufferSize);	//	@8AADA0
	~TransactionBuffer();	//	@875210

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, false);
	}
};

static_assert(sizeof(TransactionBuffer) == REWINDBUFFER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TransactionBuffer));