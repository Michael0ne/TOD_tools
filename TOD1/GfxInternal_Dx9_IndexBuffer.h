#pragma once
#include "MemoryManager.h"

struct GfxInternal_Dx9_IndexBuffer
{
	int m_TotalIndicies;
	int m_IsTriangle;
	D3DPRIMITIVETYPE m_PrimitiveType;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	IDirect3DIndexBuffer9* m_Direct3DIndexBuffer;

public:
	void* operator new(size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, false);
		ptr = nullptr;
	}
};

ASSERT_CLASS_SIZE(GfxInternal_Dx9_IndexBuffer, 32);