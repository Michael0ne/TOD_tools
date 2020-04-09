#pragma once

#include "stdafx.h"

#include "List.h"
#include "MemoryAllocators.h"

#define FONT_CLASS_SIZE 100

class Font
{
private:
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	float field_18;
	float field_1C;
	float field_20;
	float field_24;
	float field_28;
	float field_2C;
	int field_30;
	float field_34;
	int field_38;
	int field_3C;
	List<String> m_UnkList;
	int field_50;
	int field_54;
	float field_58;
	float field_5C;
	float field_60;

public:
	Font()
	{
		debug("Font created at %X\n", this);
	}

	~Font()
	{
		debug("Font destroyed!\n");
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

	static void* _A1B698;

	static void	MakeCharactersMap();	//	@938E10
	void	Init();	//	@41EA80
};

extern Font* g_Font;

static_assert(sizeof(Font) == FONT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Font"));