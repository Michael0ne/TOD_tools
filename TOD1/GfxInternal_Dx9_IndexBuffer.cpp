#include "GfxInternal_Dx9_IndexBuffer.h"

std::map<int, GfxInternal_Dx9_IndexBuffer>* GfxInternal_Dx9_IndexBuffer::IndexBufferMap;

void GfxInternal_Dx9_IndexBuffer::CreateIndexBufferMap()
{
	IndexBufferMap = new std::map<int, GfxInternal_Dx9_IndexBuffer>;
}