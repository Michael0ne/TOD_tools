#pragma once
#include "StringsPool.h"
#include "GfxInternal_Dx9_VertexBuffer.h"
#include "GfxInternal_Dx9_IndexBuffer.h"

class MeshBuffer
{
protected:
	int field_0;
	int field_4;
	int field_8;
	GfxInternal_Dx9_VertexBuffer* m_VertexBuffer;
	GfxInternal_Dx9_IndexBuffer* m_IndexBuffer;
	String field_14;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;

public:
	MeshBuffer();	//	@
	~MeshBuffer();	//	@4616F0
};

ASSERT_CLASS_SIZE(MeshBuffer, 68);