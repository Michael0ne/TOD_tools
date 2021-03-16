#include "Buffer92.h"

RenderBuffer92::RenderBuffer92(unsigned int a1, unsigned char a2, unsigned int a3)
{
	MESSAGE_CLASS_CREATED(RenderBuffer92);

	field_14 = (a2 ^ field_14) & 0x40 ^ field_14;
	
	m_RenderBuffer[0] = { a1, ((field_14 & 0x40) != 0 ? RENDERLIST : DEFAULT) };
	m_RenderBuffer[1] = { 0, ((field_14 & 0x40) != 0 ? RENDERLIST : DEFAULT) };
	m_RenderBuffer[2] = { 0, ((field_14 & 0x40) != 0 ? RENDERLIST : DEFAULT) };

	//	NOTE: wtf is this?
	field_14 = field_14 & 0xFFFFFFF6 | a2 & 8 | ((a2 & 1) == 0);
	field_14 = field_14 ^ ((unsigned char)field_14 ^ (unsigned char)(4 * (((a2 & 4) != 0) || ((field_14 & 8) != 0)))) & 4;
	field_14 = field_14 & 0xFFFFFFCD | (2 * (((a2 & 2) != 0) || ((field_14 & 4) == 0)));
	field_0 = NULL;
	field_54 = NULL;
	field_58 = NULL;
}

RenderBuffer92::~RenderBuffer92()
{
	MESSAGE_CLASS_DESTROYED(RenderBuffer92);
}