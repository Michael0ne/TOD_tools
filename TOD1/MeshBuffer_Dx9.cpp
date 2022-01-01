#include "MeshBuffer_Dx9.h"
#include "GfxInternal_Dx9.h"

LPDIRECT3DVERTEXDECLARATION9 MeshBuffer_Dx9::StaticVertexDeclaration[4];
bool MeshBuffer_Dx9::StaticVertexDeclarationCreated;

MeshBuffer_Dx9::~MeshBuffer_Dx9()
{
	MESSAGE_CLASS_DESTROYED(MeshBuffer_Dx9);
}

void MeshBuffer_Dx9::SetMeshDataAsCurrent() const
{
	g_GfxInternal_Dx9->m_Direct3DDevice->SetStreamSource(0, m_VertexBuffer->m_Direct3DVertexBuffer, 0, m_VertexBuffer->m_Stride);
	g_GfxInternal_Dx9->SetVertexBuffer(m_VertexBuffer);

	g_GfxInternal_Dx9->m_Direct3DDevice->SetIndices(m_IndexBuffer->m_Direct3DIndexBuffer);
	g_GfxInternal_Dx9->SetIndexBuffer(m_IndexBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
void MeshBuffer_Dx9::SWSkin()
{
}

char* MeshBuffer_Dx9::LockAndGetVertexBufferPtr(const int mode)
{
	return m_VertexBuffer->LockAndGetBufferPtr(mode);
}

void MeshBuffer_Dx9::ResetStreamSources()
{
	g_GfxInternal_Dx9->m_Direct3DDevice->SetStreamSource(NULL, m_VertexBuffer->m_Direct3DVertexBuffer, NULL, m_VertexBuffer->m_Stride);
	g_GfxInternal_Dx9->SetVertexBuffer(m_VertexBuffer);

	g_GfxInternal_Dx9->m_Direct3DDevice->SetIndices(m_IndexBuffer->m_Direct3DIndexBuffer);
	g_GfxInternal_Dx9->SetIndexBuffer(m_IndexBuffer);
}

void MeshBuffer_Dx9::UnlockBuffers()
{
	if (m_VertexBuffer->m_LockMode)
		m_VertexBuffer->UnlockBuffer();

	if (m_IndexBuffer->m_LockMode)
		m_IndexBuffer->UnlockBuffer();
}

void MeshBuffer_Dx9::GetPositionFromVertexBuffer(Vector4f& outPos, const unsigned int vertexindex) const
{
	char* vb = m_VertexBuffer->GetBufferPtr();
	outPos = { *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex)), *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex) + 4), *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex) + 8), 0 };
}

void MeshBuffer_Dx9::GetNormalFromVertexBuffer(Vector4f& outNormal, const unsigned int vertexindex) const
{
	char* vb = m_VertexBuffer->GetBufferPtr();
	outNormal = { *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex) + 12), *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex) + 16), *((float*)vb + (m_VertexBuffer->m_Stride * vertexindex) + 20), 0 };
}

void MeshBuffer_Dx9::SetPositionAndNormal(const unsigned int vertexindex, const Vector3f& pos, const Vector3f& norm)
{
	float* vb = (float*)m_VertexBuffer->GetBufferPtr() + (vertexindex * m_VertexBuffer->m_Stride);

	*(Vector3f*)vb = pos;
	vb += sizeof(Vector3f) / sizeof(float); // NOTE: this is in case I f'd up the argument types.

	*(Vector3f*)vb = norm;
}

const unsigned short MeshBuffer_Dx9::GetFaceIndex(const unsigned int vertexindex) const
{
	return m_IndexBuffer->GetBufferPtr()[vertexindex];
}

void MeshBuffer_Dx9::CreateStaticVertexDeclaration()
{
	D3DVERTEXELEMENT9 Vert1[] =
	{
	 { NULL, NULL, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, NULL },
	 { NULL, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, NULL },
	 { NULL, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, NULL },
	 { 1, NULL, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, NULL },
	 D3DDECL_END()
	};

	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexDeclaration(Vert1, &StaticVertexDeclaration[0]);

	D3DVERTEXELEMENT9 Vert2[] =
	{
	 { NULL, NULL, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, NULL },
	 { NULL, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, NULL },
	 { NULL, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },
	 { NULL, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, NULL },
	 { 1, NULL, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, NULL },
	 D3DDECL_END()
	};

	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexDeclaration(Vert2, &StaticVertexDeclaration[1]);

	D3DVERTEXELEMENT9 Vert3[] =
	{
	 { NULL, NULL, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, NULL },
	 { NULL, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, NULL },
	 { NULL, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, NULL },
	 { NULL, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	 { 1, NULL, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, NULL },
	 D3DDECL_END()
	};

	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexDeclaration(Vert3, &StaticVertexDeclaration[2]);

	D3DVERTEXELEMENT9 Vert4[] =
	{
	 { NULL, NULL, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, NULL },
	 { NULL, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, NULL },
	 { NULL, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },
	 { NULL, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, NULL },
	 { NULL, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	 { 1, NULL, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, NULL },
	 D3DDECL_END()
	};

	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexDeclaration(Vert4, &StaticVertexDeclaration[3]);
}