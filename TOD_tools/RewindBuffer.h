#pragma once

#include "stdafx.h"

class Entity;

struct RewindBuffer_UnkStr
{
	int field_0;
	void* m_pUnkBuffer_Size32;
	int field_8;

	void	Init();	//	@8751D0
};

#define REWINDBUFFER_CLASS_SIZE 36

#define REWINDBUFFER_DEFAULT_SIZE 614400	//	0x96000

class RewindBuffer
{
private:
	void* m_pBuffer;
	int m_nChunks;
	int m_nSize;
	int field_C;
	RewindBuffer_UnkStr m_pUnkStr;
	void* m_pUnkPtrToUnkStr;
	Entity* field_20;

public:
	RewindBuffer()
	{
		m_pBuffer = nullptr;
		m_nChunks = 0;
		m_nSize = 0;
		field_C = 0;
		m_pUnkStr = RewindBuffer_UnkStr();
		m_pUnkPtrToUnkStr = nullptr;
		field_20 = 0;

		debug("RewindBuffer created at %X\n", this);
	}

	~RewindBuffer()
	{
		debug("RewindBuffer destroyed!\n");
	}

	void Init(unsigned int size);	//	@8AADA0
};

static_assert(sizeof(RewindBuffer) == REWINDBUFFER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(RewindBuffer));