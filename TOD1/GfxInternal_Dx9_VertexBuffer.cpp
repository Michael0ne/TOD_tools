#include "GfxInternal_Dx9_VertexBuffer.h"
#include "GfxInternal_Dx9.h"

const GfxInternal_Dx9_VertexBuffer::VertexDeclaration GfxInternal_Dx9_VertexBuffer::VertexDeclarations[] =
{
	{ 20, D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
	{ 24, D3DFVF_TEX1 | D3DFVF_XYZRHW},
	{ 28, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
	{ 24, D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 16, D3DFVF_DIFFUSE | D3DFVF_XYZ },
	{ 28, D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 32, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 40, D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 24, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ },
	{ 36, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 48, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 40, D3DFVF_NORMAL | D3DFVF_XYZ }
};
std::map<int, GfxInternal_Dx9_VertexBuffer*>* GfxInternal_Dx9_VertexBuffer::VertexBufferMap;

GfxInternal_Dx9_VertexBuffer::GfxInternal_Dx9_VertexBuffer(int FVFindex, int size, int flags)
{
	m_BufferPtr = nullptr;
	m_Direct3DVertexBuffer = nullptr;
	field_16 = NULL;
	m_InitialVerticiesCapacity = size;
	m_Stride = VertexDeclarations[FVFindex].m_Stride;
	m_FVF = VertexDeclarations[FVFindex].m_FVF;
	m_Length = size * m_Stride;
	m_VerticiesTotal = NULL;
	m_Flags = flags;
	m_FVFIndex = FVFindex;
	field_20 = NULL;
	m_BufferPtr = new char[m_Length];

	CreateVertexBuffer();
	VertexBufferMap->insert({ 0, this });	//	TODO: is this correct?
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_VertexBuffer::~GfxInternal_Dx9_VertexBuffer()
{
	MESSAGE_CLASS_DESTROYED(GfxInternal_Dx9_VertexBuffer);

	//	TODO: remove this from 'VertexMap'.

	if (m_Direct3DVertexBuffer)
	{
		m_Direct3DVertexBuffer->Release();
		m_Direct3DVertexBuffer = nullptr;
	}

	delete[] m_BufferPtr;
}

int GfxInternal_Dx9_VertexBuffer::SetData(const unsigned int verticies, const void* indata, void* outdata)
{
	if (m_VerticiesTotal + verticies >= m_InitialVerticiesCapacity)
	{
		if (outdata)
			m_Direct3DVertexBuffer->Lock(0, verticies * m_Stride, &outdata, 4096);
		else
			m_Direct3DVertexBuffer->Lock(0, 0, &outdata, 8192);

		m_VerticiesTotal = verticies;
	}
	else
	{
		m_Direct3DVertexBuffer->Lock(m_VerticiesTotal * m_Stride, verticies * m_Stride, &outdata, 4096);
		m_VerticiesTotal += verticies;
	}

	memcpy(outdata, indata, verticies * m_Stride);
	m_Direct3DVertexBuffer->Unlock();

	return m_VerticiesTotal - verticies;
}

void GfxInternal_Dx9_VertexBuffer::CreateVertexBuffer()
{
	LPDIRECT3DVERTEXBUFFER9 vertbuff = nullptr;
	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexBuffer(m_Length, m_Flags & 1 ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY : D3DUSAGE_WRITEONLY, m_FVF, D3DPOOL_DEFAULT, &vertbuff, nullptr);

	if (vertbuff != m_Direct3DVertexBuffer)
		m_Direct3DVertexBuffer->Release();

	m_Direct3DVertexBuffer = vertbuff;
}

void GfxInternal_Dx9_VertexBuffer::CreateVerticesMap()
{
	VertexBufferMap = new std::map<int, GfxInternal_Dx9_VertexBuffer*>();
}