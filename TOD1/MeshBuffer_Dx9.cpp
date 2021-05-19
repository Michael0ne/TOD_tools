#include "MeshBuffer_Dx9.h"
#include "GfxInternal_Dx9.h"

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