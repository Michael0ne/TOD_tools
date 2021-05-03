#pragma once
#include "MemoryManager.h"
#include <map>

struct GfxInternal_Dx9_VertexBuffer
{
	struct VertexDeclaration
	{
		unsigned int	m_Stride;
		unsigned int	m_FVF;

		VertexDeclaration(unsigned int s, unsigned int f)
			: m_Stride(s), m_FVF(f)
		{};
	};

	unsigned int				m_InitialVerticiesCapacity;
	int							m_VerticiesTotal;
	int							m_FVF;
	int							m_Length;
	char* m_BufferPtr;
	short						m_Stride;
	char						field_16;
	int							m_FVFIndex;
	int							m_Flags;
	int							field_20;
	LPDIRECT3DVERTEXBUFFER9		m_Direct3DVertexBuffer;

public:
	GfxInternal_Dx9_VertexBuffer(int FVFindex, int size, int flags);	//	@464E70
	~GfxInternal_Dx9_VertexBuffer();	//	@465070

	void* operator new(size_t size)
	{
		if (!MemoryManager::Released)
			return MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::AllocatorsList[DEFAULT]->Free(ptr);
		ptr = nullptr;
	}

	int							SetData(const unsigned int verticies, const void* indata, void* outdata);	//	@464C00
	void						CreateVertexBuffer();	//	@464CC0

	static void					CreateVerticesMap();	//	@4651B0

	static const VertexDeclaration	VertexDeclarations[];	//	@A0ABD0
	static std::map<int, GfxInternal_Dx9_VertexBuffer*>* VertexBufferMap;	//	@A39F58
};

ASSERT_CLASS_SIZE(GfxInternal_Dx9_VertexBuffer, 40);