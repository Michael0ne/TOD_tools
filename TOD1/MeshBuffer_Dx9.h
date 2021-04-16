#pragma once
#include "StringsPool.h"
#include <vector>

//	TODO: this is inherited from another class, don't know what right now...
class MeshBufferDx9
{
private:
	void*	field_0;	//	NOTE: pointer to 'Buffer108'.
	int	field_4;
	int	field_8;
	GfxInternal_Dx9_VertexBuffer*	m_VertexBuffer;	//	NOTE: this and one below are pointers to something.
	GfxInternal_Dx9_IndexBuffer*	m_IndexBuffer;
	String field_14;

	float	field_24;
	float	field_28;
	float	field_2C;
	float	field_30;

	float	field_34;
	float	field_38;
	float	field_3C;
	float	field_40;

	std::vector<int>	field_44;	//	NOTE: probably, these are the only members of this class.
	std::vector<int>	field_54;

public:
	MeshBufferDx9(void* a1, int a2);	//	@44BE80
	MeshBufferDx9(const void* mutableBuffer, int a2);	//	@4618D0
	~MeshBufferDx9();	//	@44BBF0
};

ASSERT_CLASS_SIZE(MeshBufferDx9, 100);